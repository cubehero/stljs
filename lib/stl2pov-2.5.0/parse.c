/* -*- c -*-
 * Time-stamp: <2009-03-28 09:50:44 rsmith>
 * 
 * stl2pov/parse.c
 * Copyright © 2004--2009 R.F. Smith <rsmith@xs4all.nl>. 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
/* If you don't want debugging and assertions, #define NDEBUG */
/*#define NDEBUG*/
/* If you don't want warning messages, #define QUIET */
/*#define QUIET*/
#include "macros.h"
#include "parse.h"
#include "utils.h"

float epsilon2 = 0.0001;
static const char *vt = "vertex";
static const char *fc = "endfacet";
static const char *nl = "normal";
static const float cflimit = 0.7071067811865475244;
static const float dimlimit = 0.50;
static const float eps = 0.01;

enum values {
	FMT_BINARY = 1, FMT_TEXT = 2, WORKSLICE = CLOCKS_PER_SEC/2
};

struct stl_reader *stl_reader_init(char *fname);
int stl_reader_step(struct stl_reader *r);
static int step_binary(struct stl_reader *r);
static int step_text(struct stl_reader *r);
static int read_text_vector(struct stl_reader *r, const char *name, 
			    float xyz[3]);
static int vertex_lookup(struct stl_reader *r, const float xyz[3]);
static int vertex_add(struct stl_reader *r, const float xyz[3]);
static void vertex_ref(struct stl_reader *r, const int num);
static int edge_add(struct stl_reader *r, const int vnum[2]);
static void edge_add3(struct stl_reader *r, const int vnum[3], int edges[3]);
static void edge_ref(struct stl_reader *r, const int num, const int facet);
static int facet_add(struct stl_reader *r, const int vertex[3], 
		     const float normal[3]);

struct stl_reader *stl_reader_init(char *fname)
{
	struct stl_reader *pstl = NULL;
	int n = 0; 
/* 	int nf; */
	char *p;
	assert(fname!=NULL);
	pstl = (struct stl_reader *)calloc(sizeof(struct stl_reader), 1);
	if (pstl == NULL) {
		return NULL;
	}
	pstl->buf = ftobuf(fname, &pstl->buflen);
	if (pstl->buf == NULL) {
		free(pstl);
		return NULL;
	}
	pstl->ptr = pstl->buf;
	if (strstr(pstl->buf+84, vt)!=NULL) {	/* text format */
		pstl->fmt = FMT_TEXT;
		sscanf(pstl->ptr, "solid %80s %n", pstl->name, &n);
		pstl->ptr += n;
		p = pstl->ptr;
		while ((p = strstr(p, fc)) != NULL) {
			p += strlen(fc);
			pstl->maxnf++;
		}
	} else {   /* binary format */
		pstl->fmt = FMT_BINARY;
		pstl->maxnf = ((int)pstl->buflen-84)/50;
		memcpy(pstl->name, pstl->ptr, 84);
		for (n=0; n<84; n++) {
			if (!isprint(pstl->name[n])) {
				pstl->name[n] = 0; 
				break;
			} else if (isspace(pstl->name[n])
			    || !isalnum(pstl->name[n])) {
				pstl->name[n] = '_';
			}
                }
		pstl->ptr = pstl->buf+84;
	}
	/* allocate for most pessimistic case. */
	pstl->v = calloc(3*pstl->maxnf, sizeof(struct vertex));
	pstl->e = calloc(3*pstl->maxnf, sizeof(struct edge));
	pstl->f = calloc(pstl->maxnf, sizeof(struct facet));
	if (pstl->v==NULL || pstl->e==NULL || pstl->f==NULL) {
		free(pstl->v);
		free(pstl->e);
		free(pstl->f);
		free(pstl->buf);
		free(pstl);
		return NULL;		
	}
	return pstl;
}

int stl_reader_step(struct stl_reader *r)
{
	int n = -1;
	assert(r!=NULL);
	if (r->fmt == FMT_TEXT) {
		n =  step_text(r);
	} else if (r->fmt == FMT_BINARY) {
		n =  step_binary(r);
	}
	r->pdone = (100.0*(float)r->nf)/((float)r->maxnf);
	if (n) {
		free(r->buf);
		r->buf = r->ptr = NULL;
		r->buflen = 0;
	}
	return n;
}

void stl_reader_delete(struct stl_reader *r)
{
	assert(r!=NULL);
	free(r->v);
	free(r->e);
	free(r->f);
	if (r->buf) free(r->buf);
}

int step_binary(struct stl_reader *r)
{
	float *normal, *vertex1, *vertex2, *vertex3;
	clock_t begin, end;
	unsigned usec;
	int vertices[3];
	assert(r!=NULL);
	begin = clock();
 step_binary_again:
	if ((r->ptr - r->buf + 50) > r->buflen) return 1;
	normal = (float*) r->ptr; vertex1 = normal + 3; 
	vertex2 = vertex1 + 3; vertex3 = vertex2 + 3;
	r->ptr += 50;
	vertices[0] = vertex_add(r, vertex1);
	vertices[1] = vertex_add(r, vertex2);
	vertices[2] = vertex_add(r, vertex3);
	facet_add(r, vertices, normal);
	end = clock();
	usec = end-begin;
	if (usec < WORKSLICE)
		goto step_binary_again;
	return 0;
}

int step_text(struct stl_reader *r)
{
	float norm[3], pnt[3];
	int j, vertices[3];
	clock_t begin, end;
	unsigned usec;
	assert(r!=NULL);
	begin = clock();
 step_text_again:
	if (read_text_vector(r, nl, norm)) {
		return 1;
	}
	for (j=0; j<3; j++) {
		if (read_text_vector(r, vt, pnt)) {
			return 1;
		}
		vertices[j] = vertex_add(r, pnt);
	}
	facet_add(r, vertices, norm);
	end = clock();
	usec = end-begin;
	if (usec < WORKSLICE) 
		goto step_text_again;
     	return 0;
}

int read_text_vector(struct stl_reader *r, const char *name, float xyz[3])
{
	char *ptr, *endptr;
	int i;
	assert(r!=NULL);
	assert(name!=NULL);

	ptr = strstr(r->ptr, name);
	if (ptr==NULL) return 1;
	ptr += strlen(name);
	for (i=0; i<3; i++) {
		xyz[i] = (float)strtod(ptr, &endptr);
		if (endptr == ptr) {
			return 2;
		}
		ptr = endptr;
	}
	r->ptr = ptr;
	return 0;
}



int vertex_lookup(struct stl_reader *r, const float xyz[3])
{
	int i;
	float dist2;
	assert(r!=NULL);
	/* Check if there is another point within sqrt(epsilon2) and return
	 * it if found. */
	for (i=0; i<r->nv; i++) {
		dist2 = powf(r->v[i].xyz[0]-xyz[0], 2.0) +
			powf(r->v[i].xyz[1]-xyz[1], 2.0) +
			powf(r->v[i].xyz[2]-xyz[2], 2.0);
		if (dist2 < epsilon2) {
			return i;
		}
	}
	/* If no point found, return -1. */
	return -1;
}

int vertex_add(struct stl_reader *r, const float xyz[3])
{
	int num = -1;
	struct vertex *pnw = NULL;
	assert(r!=NULL);
	/* Check if the vertex already exists. */
	num = vertex_lookup(r, xyz);
	if (num == -1) {
		if (r->nv == r->maxnf*3) {
			fatal("Too many vertices\n");
		}
		num = r->nv++;
		pnw = r->v+num;
		/* If not, add the vertex. */
		pnw->xyz[0] = xyz[0];
		pnw->xyz[1] = xyz[1];
		pnw->xyz[2] = xyz[2];
		pnw->refcnt = 0;
	}
	/* Return the vertex number. */
	return num;	
}

void vertex_ref(struct stl_reader *r, const int num)
{
	assert(r!=NULL);
	assert(num>-1);
	assert(num<r->nv);
	r->v[num].refcnt++;
}

int edge_add(struct stl_reader *r, const int vnum[2])
{
	int i, num = -1;
	assert(r!=NULL);
	/* Check if the edge is already in the array */
	for (i=0; i<r->ne; i++) {
		if ((r->e[i].vertex[0]==vnum[0]&&r->e[i].vertex[1]==vnum[1]) ||
		    (r->e[i].vertex[0]==vnum[1]&&r->e[i].vertex[1]==vnum[0])) {
			num = i;
			break;
		}
	}
	/* If not, append to the array. */
	if (num == -1) {
		if (r->ne == r->maxnf*3) {
			fatal("Too many edges\n");
		}
		num = r->ne++;
		r->e[num].vertex[0] = vnum[0];
		r->e[num].vertex[1] = vnum[1];
		r->e[num].refcnt = 0;
	}
	return num;
}

void edge_add3(struct stl_reader *r, const int vnum[3], int edges[3])
{
	int v[2];
	assert(r!=NULL);
	assert(vnum[0]>-1);
	assert(vnum[0]<r->nv);
	assert(vnum[1]>-1);
	assert(vnum[1]<r->nv);
	assert(vnum[2]>-1);
	assert(vnum[2]<r->nv);
	/* Add first edge */
	v[0] = vnum[0];
	v[1] = vnum[1];
	edges[0] = edge_add(r, v);
	/* Add second edge */
	v[0] = vnum[1];
	v[1] = vnum[2];
	edges[1] = edge_add(r, v);
	/* Add third edge */
	v[0] = vnum[2];
	v[1] = vnum[0];
	edges[2] = edge_add(r, v);	
}

void edge_ref(struct stl_reader *r, const int num, const int facet)
{
	assert(r!=NULL);
	assert(num>-1);
	assert(num<r->ne);
	r->e[num].refcnt++;
}

int facet_add(struct stl_reader *r, const int vertex[3], const float normal[3])
{
        int num = -1;
        struct facet nw = {{0, 0, 0}, {0, 0, 0}, {0.0, 0.0, 0.0}};
	assert(r!=NULL);
	assert(vertex[0]>-1);
        assert(vertex[1]>-1);
        assert(vertex[2]>-1);
        assert(vertex[0]<r->nv);
        assert(vertex[1]<r->nv);
        assert(vertex[2]<r->nv);
        /* Fill the vertices and the normal vector */
        nw.vertex[0] = vertex[0];
        nw.vertex[1] = vertex[1];
        nw.vertex[2] = vertex[2];
        nw.normal[0] = normal[0];
        nw.normal[1] = normal[1];
        nw.normal[2] = normal[2];
        /* Add the edges to the edge array. */
        edge_add3(r, vertex, nw.edges);
	/* Add the facet to the array. */
	if (r->nf == r->maxnf) {
		fatal("Too many edges\n");
	}
	memcpy(r->f+r->nf++, &nw, sizeof(struct facet));
	/* Up the reference counts. */
        vertex_ref(r, vertex[0]);
        vertex_ref(r, vertex[1]);
        vertex_ref(r, vertex[2]);
        edge_ref(r, nw.edges[0], num);
        edge_ref(r, nw.edges[1], num);
        edge_ref(r, nw.edges[2], num);
	return r->nf;
}

int calc_vertex_normal(struct stl_reader *r, int fi, int vi, float normal[3])
{
	int i, ref = 0;
	float n[3];
	assert(fi>-1);
	assert(vi>-1);
	assert(fi<r->nf);
	assert(vi<r->nv);
	n[0] = r->f[fi].normal[0];
	n[1] = r->f[fi].normal[1];
	n[2] = r->f[fi].normal[2];
	for (i=0; i<r->nf && ref<r->v[vi].refcnt; i++) {
		if (i!=fi && (r->f[i].vertex[0]==vi
		    ||r->f[i].vertex[1]==vi
		    ||r->f[i].vertex[2]==vi)) {
			ref++;
			if (cosphi(r->f[i].normal,r->f[fi].normal)>cflimit 
			    && maxdim(r, i)/maxdim(r, fi)>dimlimit) {
				n[0] += r->f[i].normal[0];
				n[1] += r->f[i].normal[1];
				n[2] += r->f[i].normal[2];
			}
		}
	}
	normalize(n);
	memcpy(normal, n, sizeof(float[3]));
	if (fabsf(n[0]-r->f[fi].normal[0])>eps 
	    || fabsf(n[1]-r->f[fi].normal[1])>eps 
	    || fabsf(n[2]-r->f[fi].normal[2])>eps)
		return 1; /* vertex normal _not_ equal to facet normal */
	return 0; /* vertex normal equal to facet normal */
}

float maxdim(struct stl_reader *r, int fi)
{
	float p0[3], p1[3], p2[3], d0, d1, d2, max;
	int i;
	i = r->f[fi].vertex[0];
	memcpy(p0, r->v+i, sizeof(float[3]));
	i = r->f[fi].vertex[1];
	memcpy(p1, r->v+i, sizeof(float[3]));
	i = r->f[fi].vertex[2];
	memcpy(p2, r->v+i, sizeof(float[3]));
	d0 = sqrtf(powf(p1[0]-p0[0],2)+powf(p1[1]-p0[1],2)
		   +powf(p1[2]-p0[2],2));
	d1 = sqrtf(powf(p2[0]-p1[0],2)+powf(p2[1]-p1[1],2)
		   +powf(p2[2]-p1[2],2));
	d2 = sqrtf(powf(p0[0]-p2[0],2)+powf(p0[1]-p2[1],2)
		   +powf(p0[2]-p2[2],2));
	max = (d0>d1)?d0:d1;
	return (d2>max)?d2:max;
}
/* EOF parse.c */
