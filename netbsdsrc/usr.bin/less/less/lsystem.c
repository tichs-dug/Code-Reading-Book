/*	$NetBSD: lsystem.c,v 1.1.1.3 1997/09/21 12:23:17 mrg Exp $	*/

/*
 * Copyright (c) 1984,1985,1989,1994,1995,1996  Mark Nudelman
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice in the documentation and/or other materials provided with 
 *    the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR 
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN 
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


/*
 * Routines to execute other programs.
 * Necessarily very OS dependent.
 */

#include <signal.h>
#include "less.h"
#include "position.h"

#if MSDOS_COMPILER
#include <dos.h>
#ifdef _MSC_VER
#include <direct.h>
#define setdisk(n) _chdrive((n)+1)
#else
#include <dir.h>
#endif
#endif

extern int screen_trashed;
extern IFILE curr_ifile;


#if HAVE_SYSTEM

/*
 * Pass the specified command to a shell to be executed.
 * Like plain "system()", but handles resetting terminal modes, etc.
 */
	public void
lsystem(cmd, donemsg)
	char *cmd;
	char *donemsg;
{
	register int inp;
#if HAVE_SHELL
	register char *shell;
	register char *p;
#endif
	IFILE save_ifile;
#if MSDOS_COMPILER
	char cwd[FILENAME_MAX+1];
#endif

	/*
	 * Print the command which is to be executed,
	 * unless the command starts with a "-".
	 */
	if (cmd[0] == '-')
		cmd++;
	else
	{
		clear_bot();
		putstr("!");
		putstr(cmd);
		putstr("\n");
	}

#if MSDOS_COMPILER
	/*
	 * Working directory is global on MSDOS.
	 * The child might change the working directory, so we
	 * must save and restore CWD across calls to `system',
	 * or else we won't find our file when we return and
	 * try to `reedit_ifile' it.
	 */
	getcwd(cwd, FILENAME_MAX);
#endif

	/*
	 * Close the current input file.
	 */
	save_ifile = save_curr_ifile();
	(void) edit_ifile(NULL_IFILE);

	/*
	 * De-initialize the terminal and take out of raw mode.
	 */
	deinit();
	flush();	/* Make sure the deinit chars get out */
	raw_mode(0);
#if MSDOS_COMPILER==WIN32C
	close_getchr();
#endif

	/*
	 * Restore signals to their defaults.
	 */
	init_signals(0);

#if HAVE_DUP
	/*
	 * Force standard input to be the user's terminal
	 * (the normal standard input), even if less's standard input 
	 * is coming from a pipe.
	 */
	inp = dup(0);
	close(0);
	if (open("/dev/tty", OPEN_READ) < 0)
		dup(inp);
#endif

	/*
	 * Pass the command to the system to be executed.
	 * If we have a SHELL environment variable, use
	 * <$SHELL -c "command"> instead of just <command>.
	 * If the command is empty, just invoke a shell.
	 */
#if HAVE_SHELL
	p = NULL;
	if ((shell = lgetenv("SHELL")) != NULL && *shell != '\0')
	{
		if (*cmd == '\0')
			p = save(shell);
		else
		{
			p = (char *) ecalloc(strlen(shell) + strlen(cmd) + 7, 
					sizeof(char));
			sprintf(p, "%s -c \"%s\"", shell, cmd);
		}
	}
	if (p == NULL)
	{
		if (*cmd == '\0')
			p = save("sh");
		else
			p = save(cmd);
	}

	system(p);
	free(p);
#else
#if MSDOS_COMPILER==DJGPPC
	/*
	 * We don't need to catch signals of the child (it
	 * also makes trouble with some DPMI servers).
	 */
	__djgpp_exception_toggle();
  	system(cmd);
	__djgpp_exception_toggle();
#else
	system(cmd);
#endif
#endif

#if HAVE_DUP
	/*
	 * Restore standard input, reset signals, raw mode, etc.
	 */
	close(0);
	dup(inp);
	close(inp);
#endif

#if MSDOS_COMPILER==WIN32C
	open_getchr();
#endif
	init_signals(1);
	raw_mode(1);
	if (donemsg != NULL)
	{
		putstr(donemsg);
		putstr("  (press RETURN)");
		get_return();
	}
	init();
	screen_trashed = 1;

#if MSDOS_COMPILER
	/*
	 * Restore the previous directory (possibly
	 * changed by the child program we just ran).
	 */
	chdir(cwd);
#if MSDOS_COMPILER != DJGPPC
	/*
	 * Some versions of chdir() don't change to the drive
	 * which is part of CWD.  (DJGPP does this in chdir.)
	 */
	if (cwd[1] == ':')
	{
		if (cwd[0] >= 'a' && cwd[0] <= 'z')
			setdisk(cwd[0] - 'a');
		else if (cwd[0] >= 'A' && cwd[0] <= 'Z')
			setdisk(cwd[0] - 'A');
	}
#endif
#endif

	/*
	 * Reopen the current input file.
	 */
	reedit_ifile(save_ifile);

#if defined(SIGWINCH) || defined(SIGWIND)
	/*
	 * Since we were ignoring window change signals while we executed
	 * the system command, we must assume the window changed.
	 * Warning: this leaves a signal pending (in "sigs"),
	 * so psignals() should be called soon after lsystem().
	 */
	winch(0);
#endif
}

#endif

#if PIPEC

/*
 * Pipe a section of the input file into the given shell command.
 * The section to be piped is the section "between" the current
 * position and the position marked by the given letter.
 *
 * If the mark is after the current screen, the section between
 * the top line displayed and the mark is piped.
 * If the mark is before the current screen, the section between
 * the mark and the bottom line displayed is piped.
 * If the mark is on the current screen, or if the mark is ".",
 * the whole current screen is piped.
 */
	public int
pipe_mark(c, cmd)
	int c;
	char *cmd;
{
	POSITION mpos, tpos, bpos;

	/*
	 * mpos = the marked position.
	 * tpos = top of screen.
	 * bpos = bottom of screen.
	 */
	mpos = markpos(c);
	if (mpos == NULL_POSITION)
		return (-1);
	tpos = position(TOP);
	if (tpos == NULL_POSITION)
		tpos = ch_zero();
	bpos = position(BOTTOM);

 	if (c == '.') 
 		return (pipe_data(cmd, tpos, bpos));
 	else if (mpos <= tpos)
 		return (pipe_data(cmd, mpos, bpos));
 	else if (bpos == NULL_POSITION)
 		return (pipe_data(cmd, tpos, bpos));
 	else
 		return (pipe_data(cmd, tpos, mpos));
}

/*
 * Create a pipe to the given shell command.
 * Feed it the file contents between the positions spos and epos.
 */
	public int
pipe_data(cmd, spos, epos)
	char *cmd;
	POSITION spos;
	POSITION epos;
{
	register FILE *f;
	register int c;
	extern FILE *popen();

	/*
	 * This is structured much like lsystem().
	 * Since we're running a shell program, we must be careful
	 * to perform the necessary deinitialization before running
	 * the command, and reinitialization after it.
	 */
	if (ch_seek(spos) != 0)
	{
		error("Cannot seek to start position", NULL_PARG);
		return (-1);
	}

	if ((f = popen(cmd, "w")) == NULL)
	{
		error("Cannot create pipe", NULL_PARG);
		return (-1);
	}
	clear_bot();
	putstr("!");
	putstr(cmd);
	putstr("\n");

	deinit();
	flush();
	raw_mode(0);
	init_signals(0);
#if MSDOS_COMPILER==WIN32C
	close_getchr();
#endif
#ifdef SIGPIPE
	LSIGNAL(SIGPIPE, SIG_IGN);
#endif

	c = EOI;
	while (epos == NULL_POSITION || spos++ <= epos)
	{
		/*
		 * Read a character from the file and give it to the pipe.
		 */
		c = ch_forw_get();
		if (c == EOI)
			break;
		if (putc(c, f) == EOF)
			break;
	}

	/*
	 * Finish up the last line.
	 */
 	while (c != '\n' && c != EOI ) 
 	{
 		c = ch_forw_get();
 		if (c == EOI)
 			break;
 		if (putc(c, f) == EOF)
 			break;
 	}

	pclose(f);

#ifdef SIGPIPE
	LSIGNAL(SIGPIPE, SIG_DFL);
#endif
#if MSDOS_COMPILER==WIN32C
	open_getchr();
#endif
	init_signals(1);
	raw_mode(1);
	init();
	screen_trashed = 1;
#if defined(SIGWINCH) || defined(SIGWIND)
	/* {{ Probably don't need this here. }} */
	winch(0);
#endif
	return (0);
}

#endif

#ifdef _OSK
/*
 *    Popen, and Pclose, for OS-9.
 *
 *    Based on code copyright (c) 1988 by Wolfgang Ocker, Puchheim,
 *                                        Ulli Dessauer, Germering and
 *                                        Reimer Mellin, Muenchen
 *                                        (W-Germany)
 *
 *    These functions can be copied and distributed freely for any
 *    non-commercial purposes.  It can only be incorporated into
 *    commercial software with the written permission of the authors.
 *
 *    TOP-specific code stripped out and adapted for less by M.Gregorie, 1996
 *
 *    address:    Wolfgang Ocker
 *                Lochhauserstrasse 35a
 *                D-8039 Puchheim
 *                West Germany
 *
 *    e-mail:     weo@altger.UUCP, ud@altger.UUCP, ram@altger.UUCP
 *                pyramid!tmpmbx!recco!weo
 *                pyramid!tmpmbx!nitmar!ud
 *                pyramid!tmpmbx!ramsys!ram
 *
 *                Martin Gregorie
 *                10 Sadlers Mead
 *                Harlow
 *                Essex, CM18 6HG
 *                U.K.
 *
 *                gregorie@logica.com
 */
#include <strings.h>
#include <errno.h>
extern char **environ;
extern char *getenv();
extern int  os9forkc();
static int pids[_NFILE] = { 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0 };
/* 
 * p o p e n
 */
FILE *popen(name, mode)
char     *name,
        *mode;
{
    int          fd, fd2, fdsav, pid;
    static char  *argv[] = {NULL, NULL, NULL };
    static char  cmd[200];
    static char  cmd_path[200];
    char         *cp;
    char         *shell;
    FILE         *r;
    if ((shell = getenv("SHELL")) == NULL)
        return(NULL);
    cp = name;
    while (*cp == ' ')
        cp++;
    strcpy(cmd_path, cp);
    if (cp = index(cmd_path, ' '))
        *cp++ = '\0';
    strcpy(cmd, "ex ");
    strcat(cmd, cmd_path);
    if (cp)
    {
        strcat(cmd, " ");
        strcat(cmd, cp);
    }
    argv[0] = shell;
    argv[1] = cmd;
    /*
         mode is "r" (stdout) or "w" (stdin)
    */
    switch(mode[0])
    {
        case 'w':   fd = 0;
                    break;
        case 'r':   fd = 1;
                    break;
        default:    return(NULL);
    }
    if (fd == 1)
        fflush(stdout);
    fdsav = dup(fd);
    close(fd);
 
    creat("/pipe", S_IWRITE+S_IREAD);
    pid = os9exec(os9forkc, argv[0], argv, environ, 0, 0, 3);
    fd2 = dup(fd);
    close(fd);
    dup(fdsav);
    close(fdsav);
    if (pid > 0)
    {
        pids[fd2] = pid;
        r = fdopen(fd2, mode);
    }
    else
    {
        close(fd2);
        r = NULL;
    }
    return(r);
}

/*
 * p c l o s e
 */
int pclose(fp)
FILE    *fp;
{
    unsigned int    status;
    int             pid;
    int             fd,
                    i;
    fd = fileno(fp);
    if (pids[fd] == 0)
        return(-1);
    fflush(fp);
    fclose(fp);
    while ((pid = wait(&status)) != -1)
        if (pid == pids[fd])
            break;
        else
            for (i = 0; i < _NFILE; i++)
                if (pids[i] == pid)
                {
                    pids[i] = 0;
                    break;
                }
    if (pid == -1)
        status = -1;
    pids[fd] = 0;
    return(status);
}
#endif /* _OSK */
