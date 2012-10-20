/* -*- c -*-
 * Time-stamp: <2009-03-28 09:54:02 rsmith>
 * 
 * stl2pov/utils.c
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
#include <math.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "utils.h"

#ifndef PACKAGE
#error You must define PACKAGE as the name of the application!
#endif

void fatal(const char *fmt, ...)
{
	va_list ap;
	assert(fmt != NULL);
	va_start(ap, fmt);
	fputs(PACKAGE" error: ", stderr);
	vfprintf(stderr, fmt, ap);
	exit(EXIT_FAILURE);
}

void warning(const char *fmt, ...)
{
	va_list ap;
	assert(fmt != NULL);
	va_start(ap, fmt);
	fputs(PACKAGE" warning: ", stderr);
	vfprintf(stderr, fmt, ap);
}

char *ftobuf(const char *name, long *l)
{
        char *b = NULL;
        FILE *f;
	long bl = 0;
        assert(name != NULL);
        f = fopen(name, "r");
        if (f == NULL) {
		warning("Could not open %s.\n", name);
		return NULL;
	}
        fseek(f, 0, SEEK_END);
        bl = ftell(f);
        rewind(f);
        if (bl == -1) {
                fclose(f);
		warning("Could not determine the size of %s.\n", name);
		return NULL;
        }
        b = malloc(bl+1); /* for an extra zero at the end. */
	if (b == NULL) {
                fclose(f);
                warning("Could not allocate %d bytes of memory for %s.\n", 
			bl, name);
		return NULL;
	}
        fread(b, bl, 1, f);
        if (ferror(f)) {
                fclose(f);
		free(b);
		warning("Could not read %s.\n", name);
		return NULL;
        }
	b[bl] = 0;
        if (l!=NULL) {
                *l = bl;
        }
        fclose(f);
        return b;
}

void normalize(float xyz[3])
{
	float len = sqrtf(xyz[0]*xyz[0]+xyz[1]*xyz[1]+xyz[2]*xyz[2]);
	xyz[0] /= len;
	xyz[1] /= len;
	xyz[2] /= len;
}

float cosphi(const float a[3], const float b[3])
{
	float t, n;
	t = a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
	n = sqrtf(a[0]*a[0]+a[1]*a[1]+a[2]*a[2])
		* sqrtf(b[0]*b[0]+b[1]*b[1]+b[2]*b[2]);
	return t/n;
}
/* EOF utils.c */
