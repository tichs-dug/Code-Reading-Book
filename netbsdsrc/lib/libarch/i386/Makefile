#	$NetBSD: Makefile,v 1.1 1996/02/21 02:45:47 jtk Exp $

.PATH: ${LIBC}/i386

SRCS+=	i386_get_ldt.c i386_set_ldt.c \
	i386_iopl.c i386_get_ioperm.c i386_set_ioperm.c \
	i386_vm86.c

MAN+=	i386_get_ldt.2 \
	i386_iopl.2 i386_get_ioperm.2 \
	i386_vm86.2

MLINKS+=i386_get_ldt.2 i386_set_ldt.2 \
	i386_get_ioperm.2 i386_set_ioperm.2

.include <bsd.lib.mk>

