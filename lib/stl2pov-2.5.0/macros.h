/* -*- c -*-
 * Time-stamp: <2009-03-28 09:54:30 rsmith>
 * 
 * stl2pov/macros.h
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

#ifndef MACROS_H
#define MACROS_H

#ifndef NULL
#define NULL (void*)0
#endif

#ifndef RETURN_IF_FAIL
#define RETURN_IF_FAIL(test) if (!(test)) return
#endif

#ifndef RETURN_VAL_IF_FAIL
#define RETURN_VAL_IF_FAIL(test,val) if (!(test)) return ((val))
#endif

#ifndef NDEBUG
/* __FUNCTION__ and varags macros are a GCC feature. */
#  ifdef __GNUC__
#    undef debug
#    define debug(a...) fprintf(stderr,"%s, line %i, %s(): ",\
__FILE__,__LINE__,__FUNCTION__); fprintf(stderr, ## a); fprintf(stderr, "\n")
#  endif                        /* __GNUC__ */
#else
#  undef debug
#  define debug(a...) (void)0
#endif                          /* NDEBUG */

#ifndef QUIET
#  ifdef __GNUC__
#    undef warn
#    define warn(a,...) fprintf(stderr,"%s, line %i, %s(), warning: ",\
__FILE__,__LINE__,__FUNCTION__); fprintf(stderr, ## a); fprintf(stderr, "\n")
#  endif
#endif                          /* QUIET */

#endif				/* MACROS_H */
/* EOF macros.h */
