#	$NetBSD: Makefile,v 1.12 1997/09/22 22:11:00 christos Exp $

PROG= fsinfo
SRCS=	fsi_gram.c fsi_lex.c wr_fstab.c wr_exportfs.c wr_dumpset.c wr_bparam.c \
wr_atab.c fsinfo.c fsi_util.c fsi_dict.c fsi_analyze.c 
MAN+= fsinfo.8
CLEANFILES+=fsi_lex.c fsi_gram.c fsi_gram.h y.tab.h

fsi_lex.o: fsi_gram.h

fsi_gram.c fsi_gram.h: fsi_gram.y
	${YACC} -d ${.ALLSRC}
	mv y.tab.c fsi_gram.c
	mv y.tab.h fsi_gram.h

.include <bsd.prog.mk>
