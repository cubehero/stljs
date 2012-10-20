/* -*- c -*-
 * Time-stamp: <2011-06-22 20:53:32 rsmith>
 * 
 * stl2pov/license.c
 * Copyright © 2004--2009,2011 R.F. Smith <rsmith@xs4all.nl>. 
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

#include <stdio.h>

void print_license(FILE *f) 
{
	fputs("Copyright © 2004--2009,2011 R.F. Smith <rsmith@xs4all.nl>."\
	      " All rights reserved\n\n", f);
	fputs("Redistribution and use in source and binary forms, "\
	      "with or without\n", f);
	fputs("modification, are permitted provided that the following"\
	      " conditions\n", f);
	fputs("are met:\n", f);
	fputs("1. Redistributions of source code must retain the above"\
	      " copyright\n", f);
	fputs("   notice, this list of conditions and the following"\
	      " disclaimer.\n", f);
	fputs("2. Redistributions in binary form must reproduce the"\
	      " above copyright\n", f);
	fputs("   notice, this list of conditions and the following"\
	      " disclaimer in the\n", f);
	fputs("   documentation and/or other materials provided with"\
	      " the distribution.\n\n", f);
	fputs("THIS SOFTWARE IS PROVIDED BY AUTHOR AND CONTRIBUTORS"\
	      " ``AS IS'' AND\n", f);
	fputs("ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT"\
	      " LIMITED TO, THE\n", f);
	fputs("IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR"\
	      " A PARTICULAR PURPOSE\n", f);
	fputs("ARE DISCLAIMED.  IN NO EVENT SHALL AUTHOR OR CONTRIBUTORS"\
	      " BE LIABLE\n", f);
	fputs("FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,"\
	      " OR CONSEQUENTIAL\n", f);
	fputs("DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF"\
	      " SUBSTITUTE GOODS\n", f);
	fputs("OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS"\
	      " INTERRUPTION)\n", f);
	fputs("HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER"\
	      " IN CONTRACT, STRICT\n", f);
	fputs("LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)"\
	      " ARISING IN ANY WAY\n", f);
	fputs("OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE"\
	      " POSSIBILITY OF\n", f);
	fputs("SUCH DAMAGE.\n", f);
}
/* EOF license.c */
