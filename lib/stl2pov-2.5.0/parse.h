/* -*- c -*-
 * Time-stamp: <2009-03-28 09:52:04 rsmith>
 * 
 * stl2pov/parse.h
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

#ifndef PARSE_H
#define PARSE_H

#ifdef __cplusplus
#  define BEGIN_C_DECLS extern "C" {
#  define END_C_DECLS   }
#else                           /* !__cplusplus */
#  define BEGIN_C_DECLS
#  define END_C_DECLS
#endif                          /* __cplusplus */
#include <stdio.h>

BEGIN_C_DECLS

struct vertex {
	float xyz[3];
	int refcnt;		/* number of facets */
};

struct edge {
	int vertex[2];
	int facet[2];	/* facets that reference this vertex */
	int refcnt;		/* number of facets */
};

struct facet {
	int vertex[3];
	int edges[3];
	float normal[3];
};

struct stl_reader {
	char name[85];
	struct vertex *v;	/* vertex */
	struct edge *e;		/* edge */
	struct facet *f;	/* facet */
	int nv, ne, nf, maxnf;
	/* The following are for the reader state. */
	float pdone;	/* percentage */
	char *buf, *ptr;
	long buflen;
	int fmt;	/* FMT_BINARY or FMT_TEXT */
};

/* Square of the allowable distance between two points that are reckoned
 * to be the same. */
extern float epsilon2;

/* Create the reader structure. */
extern struct stl_reader *stl_reader_init(char *fname);

/* Execute a parsing step. */
extern int stl_reader_step(struct stl_reader *r);

/* Free the memory used by the reader. */
extern void stl_reader_delete(struct stl_reader *r);

/* Calculate the vertex normal for a vertex indexed by 'vi' belonging to
 * facet indexed by 'ti'. */
extern int calc_vertex_normal(struct stl_reader *r, 
			      int fi, int vi, float normal[3]);

/* Calculate the maximum dimension of a facet indexed by 'fi' */
extern float maxdim(struct stl_reader *r, int fi);

END_C_DECLS
#endif				/* PARSE_H */
/* EOF parse.h */
