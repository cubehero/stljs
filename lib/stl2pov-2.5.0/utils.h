/* -*- c -*-
 * Time-stamp: <2009-03-28 09:53:51 rsmith>
 * 
 * stl2pov/utils.h
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

#ifndef UTILS_H
#define UTILS_H

#ifdef __cplusplus
#  define BEGIN_C_DECLS extern "C" {
#  define END_C_DECLS   }
#else                           /* !__cplusplus */
#  define BEGIN_C_DECLS
#  define END_C_DECLS
#endif                          /* __cplusplus */
#include <stdio.h>

BEGIN_C_DECLS

/* Report a fatal error on the standard error stream and terminates the
 * program with EXIT_FAILURE. It does not return. */
extern void fatal(const char *fmt, ...);
/* Gives a warning on the standard error stream. */
extern void warning(const char *fmt, ...);
/* Read the contents of a file into a buffer. A kind of poor man's mmap for
 * systems that don't have one. */
extern char *ftobuf(const char *name, long *l);
/* Scales the given vector to length 1 */
extern void normalize(float xyz[3]);
/* Calculate the cosine of the angle between to vectors; a.b/(|a||b|) */
extern float cosphi(const float a[3], const float b[3]);

END_C_DECLS
#endif				/* UTILS_H */
/* EOF utils.h */
