#	$NetBSD: Makefile,v 1.18.2.1 1997/11/08 10:37:53 lukem Exp $
#	from: @(#)Makefile    8.3 (Berkeley) 4/2/94


PROG=	passwd
SRCS=	local_passwd.c yp_passwd.c passwd.c getpwent.c
.PATH:  ${.CURDIR}/../../lib/libc/gen
DPADD+= ${LIBRPCSVC} ${LIBCRYPT} ${LIBUTIL}
LDADD+= -lrpcsvc -lcrypt -lutil
CPPFLAGS+=-I${.CURDIR} -I${.CURDIR}/../../lib/libc/include -DYP

.if defined(KERBEROS5)
SRCS+= krb5_passwd.c
CPPFLAGS+= -DKERBEROS5
DPADD+=	${LIBKRB5} ${LIBCRYPTO} 
LDADD+=	-lkrb5 -lcrypto
.endif

LINKS=	${BINDIR}/passwd ${BINDIR}/yppasswd
MLINKS=	passwd.1 yppasswd.1

BINOWN=	root
BINMODE=4555
.if defined(notyet)
INSTALLFLAGS=-fschg
.endif

.include <bsd.prog.mk>

getpwent.o: getpwent.c
	${COMPILE.c} -UYP ${.IMPSRC}
