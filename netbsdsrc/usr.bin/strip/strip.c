/*	$NetBSD: strip.c,v 1.18 1997/10/19 23:30:41 lukem Exp $	*/

/*
 * Copyright (c) 1988, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
#ifndef lint
__COPYRIGHT("@(#) Copyright (c) 1988, 1993\n\
	The Regents of the University of California.  All rights reserved.\n");
#endif /* not lint */

#ifndef lint
#if 0
static char sccsid[] = "@(#)strip.c	8.3 (Berkeley) 5/16/95";
#else
__RCSID("$NetBSD: strip.c,v 1.18 1997/10/19 23:30:41 lukem Exp $");
#endif
#endif /* not lint */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <a.out.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct exec EXEC;
typedef struct nlist NLIST;

#define	strx	n_un.n_strx

int main __P((int, char *[]));

int	main __P((int, char **));
static int s_stab __P((const char *, int, EXEC *, struct stat *));
static int s_sym __P((const char *, int, EXEC *, struct stat *));
static void usage __P((void));

static int xflag = 0;
static int Xflag = 0;

int
main(argc, argv)
	int argc;
	char *argv[];
{
	int fd;
	EXEC *ep;
	struct stat sb;
	int (*sfcn)__P((const char *, int, EXEC *, struct stat *));
	int ch, errors;
	char *fn;

	sfcn = s_sym;
	while ((ch = getopt(argc, argv, "dxX")) != -1)
		switch(ch) {
		case 'x':
			xflag = 1;
			/*FALLTHROUGH*/
		case 'X':
			Xflag = 1;
			/*FALLTHROUGH*/
		case 'd':
			sfcn = s_stab;
			break;
		case '?':
		default:
			usage();
		}
	argc -= optind;
	argv += optind;

	errors = 0;
#define	ERROR(e) errno = e; errors |= 1; warn("%s:", fn); continue;
	while ((fn = *argv++) != NULL) {
		if ((fd = open(fn, O_RDWR)) < 0) {
			ERROR(errno);
		}
		if (fstat(fd, &sb)) {
			(void)close(fd);
			ERROR(errno);
		}
		if (sb.st_size < sizeof(EXEC)) {
			(void)close(fd);
			ERROR(EFTYPE);
		}
		if ((ep = (EXEC *)mmap(NULL, sb.st_size, PROT_READ|PROT_WRITE,
		    MAP_SHARED, fd, (off_t)0)) == (EXEC *)-1) {
			(void)close(fd);
			ERROR(errno);
		}
		if (N_BADMAG(*ep)) {
			munmap((caddr_t)ep, sb.st_size);
			(void)close(fd);
			ERROR(EFTYPE);
		}
		errors |= sfcn(fn, fd, ep, &sb);
		munmap((caddr_t)ep, sb.st_size);
		if (close(fd)) {
			ERROR(errno);
		}
	}
#undef ERROR
	exit(errors);
}

static int
s_sym(fn, fd, ep, sp)
	const char *fn;
	int fd;
	EXEC *ep;
	struct stat *sp;
{
	char *neweof;
	int zmagic;

	zmagic = ep->a_data &&
		 (N_GETMAGIC(*ep) == ZMAGIC || N_GETMAGIC(*ep) == QMAGIC);

	/*
	 * If no symbols or data/text relocation info and
	 * the file data segment size is already minimized, quit.
	 */
	if (!ep->a_syms && !ep->a_trsize && !ep->a_drsize) {
#if 0
		if (!zmagic)
			return 0;
		if (sp->st_size < N_TRELOFF(*ep))
#endif
			return 0;
	}

	/*
	 * New file size is the header plus text and data segments; OMAGIC
	 * and NMAGIC formats have the text/data immediately following the
	 * header.  ZMAGIC format wastes the rest of of header page.
	 */
	neweof = (char *)ep + N_TRELOFF(*ep);

#if 0
	/*
	 * Unfortunately, this can't work correctly without changing the way
	 * the loader works.  We could cap it at one page, or even fiddle with
	 * a_data and a_bss, but this only works for CLBYTES == NBPG.  If we
	 * are on a system where, e.g., CLBYTES is 8k and NBPG is 4k, and we
	 * happen to remove 4.5k, we will lose.  And we really don't want to
	 * fiddle with pages, because that breaks binary compatibility.  Lose.
	 */

	if (zmagic) {
		char *mineof;
		/*
		 * Get rid of unneeded zeroes at the end of the data segment
		 * to reduce the file size even more.
		 */
		mineof = (char *)ep + N_DATOFF(*ep);
		while (neweof > mineof && neweof[-1] == '\0')
			neweof--;
	}
#endif

	/* Set symbol size and relocation info values to 0. */
	ep->a_syms = ep->a_trsize = ep->a_drsize = 0;

	/* Truncate the file. */
	if (ftruncate(fd, neweof - (char *)ep)) {
		warn("%s:", fn);
		return 1;
	}

	return 0;
}

static int
s_stab(fn, fd, ep, sp)
	const char *fn;
	int fd;
	EXEC *ep;
	struct stat *sp;
{
	int cnt, len;
	char *nstr, *nstrbase, *p, *strbase;
	NLIST *sym, *nsym;
	NLIST *symbase;

	/* Quit if no symbols. */
	if (ep->a_syms == 0)
		return 0;

	if (N_SYMOFF(*ep) >= sp->st_size) {
		warnx("%s: bad symbol table", fn);
		return 1;
	}

	/*
	 * Initialize old and new symbol pointers.  They both point to the
	 * beginning of the symbol table in memory, since we're deleting
	 * entries.
	 */
	sym = nsym = symbase = (NLIST *)((char *)ep + N_SYMOFF(*ep));

	/*
	 * Allocate space for the new string table, initialize old and
	 * new string pointers.  Handle the extra long at the beginning
	 * of the string table.
	 */
	strbase = (char *)ep + N_STROFF(*ep);
	if ((nstrbase = malloc((u_int)*(u_long *)strbase)) == NULL) {
		warn("%s", "");
		return 1;
	}
	nstr = nstrbase + sizeof(u_long);

	/*
	 * Read through the symbol table.  For each non-debugging symbol,
	 * copy it and save its string in the new string table.  Keep
	 * track of the number of symbols.
	 */
	for (cnt = ep->a_syms / sizeof(NLIST); cnt--; ++sym) {
		/* debugging symbol? */
		if (sym->n_type & N_STAB)
			continue;
		/* empty symbol? */
		if (sym->strx == 0)
			continue;
		/* compute a pointer to its name */
		p = strbase + sym->strx;
		if (Xflag) {
			/* compiler identification? */
			if (!strcmp(p, "gcc_compiled."))
				continue;
			if (!strcmp(p, "gcc2_compiled."))
				continue;
			if (!strncmp(p, "___gnu_compiled_", 16))
				continue;
			/* Other -X tests? */
		}
		if (xflag) {
			/* static symbol? */
			if ((sym->n_type & N_EXT) == 0)
				continue;
			/* file name? */
			if ((sym->n_type & ~N_EXT) == N_FN)
				continue;
			/* Other -x tests? */
		}
		/* OK, copy the symbol. */
		*nsym = *sym;
		nsym->strx = nstr - nstrbase;
		len = strlen(p) + 1;
		memmove(nstr, p, len);
		nstr += len;
		++nsym;
	}

	/* Fill in new symbol table size. */
	ep->a_syms = (nsym - symbase) * sizeof(NLIST);

	/* Fill in the new size of the string table. */
	*(u_long *)nstrbase = len = nstr - nstrbase;

	/*
	 * Copy the new string table into place.  Nsym should be pointing
	 * at the address past the last symbol entry.
	 */
	memmove((void *)nsym, nstrbase, len);
	free(nstrbase);

	/* Truncate to the current length. */
	if (ftruncate(fd, (char *)nsym + len - (char *)ep)) {
		warn("%s", fn);
		return 1;
	}

	return 0;
}

static void
usage()
{
	extern char *__progname;

	(void)fprintf(stderr, "Usage: %s [-dxX] file ...\n", __progname);
	exit(1);
}
