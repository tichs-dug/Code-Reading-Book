#	$NetBSD: Makefile,v 1.4 1996/10/22 19:43:13 explorer Exp $

.include "../Makefile.inc"

.PATH:	$S/compat/freebsd $S/arch/i386/i386

CFLAGS+=	-DVM86

NOMAN=	yep

KMOD=	compat_freebsd

SRCS=	freebsd_exec.c freebsd_misc.c freebsd_sysent.c freebsd_file.c
SRCS+=	freebsd_ptrace.c freebsd_ioctl.c freebsd_syscalls.c

SRCS+=	freebsd_machdep.c

SRCS+=	lkminit_emul.c

.include <bsd.kmod.mk>
