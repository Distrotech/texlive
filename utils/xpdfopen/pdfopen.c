/*
 * pdfopen.c.
 *
 * Heavily modified by Jim Diamond (jim.diamond@acadiau.ca) 2010/04/11
 * to work with AR 9 and xpdf.
 * (Also some mods by Peter Breitenlohner <tex-live@tug.org> from 2009
 * and suggestions from 2010, and some suggestions from Karl Berry
 * (2011).)
 *
 * Note 1: the calling sequence was changed considerably to match what
 * the program actually seemed to do as well as what made sense to me.
 *
 * Note 2: if there are two instances of acroread displaying that file,
 * only one is reloaded.
 *
 * Further modified 2010/05/03 to work with ar5, and to include some code
 * that does *not* work with okular (see note below).
 * 
 * Note 3: Unlike xpdf and AR[5,7,8,9], okular will not accept X events
 * from this program unless the okular window has focus.  On my system
 * (Slackware64 13.0, using fvwm2), the functions I added to sendx.c to try
 * to change the focus to the okular window do not work.  (See also the
 * comments in sendx.c.)  If anyone cares to contribute working code,
 * please send it to me for inclusion.
 */

#define     VERSION		"0.83"

#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <unistd.h>
#include    <sys/types.h>
#include    <sys/wait.h>

#include    "sendx.h"
#include    "xpdfopen.h"
#include    "utils.h"

#define     MAX_OPTIONS		10
#define     MAX_RELOAD_CMDS	10

/* A string unlikely to be used as a real command by a PDF viewer */
#define     FORK_EXEC_CMD	"*FORK EXEC*"


/* Used in the pdf_viewer_t type... see comment below */
typedef enum
{
    USE_SENDX, USE_FORK_EXEC
} reload_method_t;


/*
 * This struct contains the information needed for both
 * (a) initially starting up a PDF viewer, and
 * (b) telling the viewer to reload the document.
 * Of course, the viewer has to support that functionality.
 * Two possible ways are supported.
 * (a) Send the viewer's window (a) simulated keypress(ss)
 * (b) fork()/exec() a second instance of the program which somehow signals
 *     the running instance.  Note that this is problematical if the
 *     arguments for the "reload" case are different than the options
 *     for the "start up the viewer" case, because the command-line
 *     (and environment variable) syntax have to be extended.
 *     Arguably, if both the start-up and reload can be done with shell
 *     commands, the user should just write a shell script and be done with
 *     it.
 * These could be extended with this as well:
 * (c) send a Unix signal to the currently running viewer.
 *     This would require augmenting reload_method_t and pdf_viewer_t.
 */

typedef struct
{
    const char * short_name;			/* cmd line option name */
    const char * program_name;			/* program to exec() */
    reload_method_t method;			/* method to induce reload */
    const char * window_name;			/* when no file is viewed */
    const char * window_name_fmt;		/* fmt string for file viewed */
    const char * exec_pre_options[MAX_OPTIONS];	/* before filename in cmd */
    const char * exec_post_options[MAX_OPTIONS];/* after filename in cmd */
    const char * reload_cmds[MAX_RELOAD_CMDS];	/* cmds to reload a file */
} pdf_viewer_t;


/* Reload commands (or prefixes thereof) */
#define     CTRL	"Ctrl-"
#define     ALT		"Alt-"
#define     CTRL_ALT	"Ctrl-Alt-"
#define     FN		"Fn-"
#define     FOCUS_IN	"FOCUS-IN"
#define     FOCUS_OUT	"FOCUS-OUT"



/*
 * The first struct represents the default viewer with the default options.
 */
pdf_viewer_t pdf_viewers[] =
{
    {"ar9", "acroread", USE_SENDX,
     AR9_WIN_NAME, AR9_WIN_NAME_FMT,
     {"-openInNewInstance", NULL},
     {NULL},
     {"Ctrl-R", NULL}},
    {"ar9-tab", "acroread", USE_SENDX, 
     AR9_WIN_NAME, AR9_WIN_NAME_FMT,
     {NULL},
     {NULL},
     {"Ctrl-R", NULL}},
    {"ar8", "acroread", USE_SENDX,
     AR8_WIN_NAME, AR8_WIN_NAME_FMT,
     {NULL},
     {NULL},
     {"Ctrl-W", FORK_EXEC_CMD, NULL}},
    {"ar7", "acroread", USE_SENDX,
     AR7_WIN_NAME, AR7_WIN_NAME_FMT, 
     {NULL},
     {NULL},
     {"Ctrl-W", "Alt-Left", NULL}},
    {"ar5", "acroread", USE_SENDX,
     AR5_WIN_NAME, AR5_WIN_NAME_FMT, 
     {NULL},
     {NULL},
     {"Ctrl-W", "Alt-Left", NULL}},
    /* 
     * xpdf also allows 'xpdf -remote tex -reload', but if that xpdf server
     * is not running, rather than a failure return, a blank window is
     * opened.  This is not much good to us.
     * Using "tex-server" as the server name is an arbitrary choice.
     */
    {"xpdf", "xpdf", USE_SENDX,
     XPDF_WIN_NAME, XPDF_WIN_NAME_FMT,
     {"-remote", "tex-server", NULL},
     {NULL},
     {"r", NULL}},
    {"evince", "evince", USE_SENDX,
     EVINCE_WIN_NAME, EVINCE_WIN_NAME_FMT,
     {NULL},
     {NULL},
     {"Ctrl-R", NULL}},
#ifdef TRY_OCULAR
    {"okular", "okular", USE_SENDX,
     OKULAR_WIN_NAME, OKULAR_WIN_NAME_FMT,
     {NULL},
     {NULL},
     {FOCUS_IN, "Fn-F5", FOCUS_OUT, NULL}},
#endif
};

char * progname;		/* set in main(); NOT static! */

#define     DEFAULT_VIEWER	(pdf_viewers[0].short_name)



static void
usage(void)
{
    int i;
    int viewers = sizeof(pdf_viewers) / sizeof(pdf_viewers[0]);

    fprintf(stderr, "This is version %s of %s.\n", VERSION, progname);
    fprintf(stderr, "Usage:\n  %s [-h|--help]\n", progname);
    fprintf(stderr, "    Show this help and exit.\n");

    fprintf(stderr, "  %s [-v|--version]\n", progname);
    fprintf(stderr, "    Show the version number and exit.\n");
    
    fprintf(stderr, "  %s [-r|--reset_focus] [-viewer <prog>] <file.pdf>\n",
	    progname);
    fprintf(stderr, "    If the PDF viewer <prog> is displaying <file.pdf>, "
	    "reload that file.\n");
    fprintf(stderr, "    Otherwise call <prog> to display <file.pdf>.\n");
    fprintf(stderr, "    If '-r' or '--reset_focus' is used, attempt to");
    fprintf(stderr, " reset the input focus\n");
    fprintf(stderr, "    after calling the viewer program.\n");
    fprintf(stderr, "    The default viewer program is %s.\n", DEFAULT_VIEWER);
    fprintf(stderr, "    Implemented viewers:");
    for (i = 0; i < viewers - 1; i++)
	fprintf(stderr, " '%s'", pdf_viewers[i].short_name);
    fprintf(stderr, " and '%s'.\n", pdf_viewers[viewers - 1].short_name);
    fprintf(stderr, "    Note: ar9 uses acroread's -openInNewInstance argument,"
	    	    " ar9-tab does not.\n");
}



/*
 * Send one "command" to the given window.
 * The command can be of these forms (for X an upper case letter or arrow
 * key, such as "Left", and D one or more digits; see sendx.c):
 * - Ctrl-X
 * - Alt-X
 * - Ctrl-Atl-X
 * - Fn-FD
 * - FOCUS-[IN|OUT]
 * - <string>	(not starting with any of the above prefixes)
 * Allow other variations on the cases of "Ctrl" and "Alt".
 * 
 * Return 0 on success, non-0 otherwise.
 */

static int
send_command(const char * window_name, const char * reload_cmd)
{
    int alt_len, ctrl_len, ctrl_alt_len, fn_len, focus_len;

    if (!strcmp(reload_cmd, FORK_EXEC_CMD))
	return 999;

    ctrl_alt_len = strlen(CTRL_ALT);
    if (!strncasecmp(CTRL_ALT, reload_cmd, ctrl_alt_len))
	return sendx_controlalt_token(reload_cmd + ctrl_alt_len, window_name);

    ctrl_len = strlen(CTRL);
    if (!strncasecmp(CTRL, reload_cmd, ctrl_len))
	return sendx_control_token(reload_cmd + ctrl_len, window_name);

    alt_len = strlen(ALT);
    if (!strncasecmp(ALT, reload_cmd, alt_len))
	return sendx_alt_token(reload_cmd + alt_len, window_name);

    fn_len = strlen(FN);
    if (!strncasecmp(FN, reload_cmd, fn_len))
	return sendx_token(reload_cmd + fn_len, window_name);

    focus_len = strlen(FOCUS_IN);
    if (!strncasecmp(FOCUS_IN, reload_cmd, focus_len))
	return set_focus(window_name);

    focus_len = strlen(FOCUS_OUT);
    if (!strncasecmp(FOCUS_OUT, reload_cmd, focus_len))
	return reset_focus();

    return sendx_string(reload_cmd, window_name);
}



/*
 * Try to reload the file.
 * If not successful, try to start up a new instance of the viewer program.
 * Return 0 on success, non-0 otherwise.
 * 
 * If reset_focus is true, (attempt to) reset the input focus to the
 * window which had focus when this program was called.  This is
 * currently only implemented for the USE_SENDX case.
 */

static int
view_file(const char * filename, int viewer_index, int focus_reset)
{
    char * window_name;
    pid_t pid;
    int	success = 0;
    pdf_viewer_t pdf_viewer = pdf_viewers[viewer_index];
    const char ** reload_cmds = pdf_viewer.reload_cmds;

    /* First try to reload the file */
    switch (pdf_viewers[viewer_index].method)
    {
      case USE_SENDX:
        window_name = make_window_name(pdf_viewer.window_name_fmt, filename);
	if (window_name == NULL)
	    return 99;
	if (focus_reset)
	    (void)set_focus(window_name);
	while (*reload_cmds)
	{
	    /*
	     * Note: there is a race condition in the sendx.c code which
	     * can cause the program to crash and burn with a 'BadWindow'
	     * fault.  This has been observed for AR8 (but not AR7, oddly)
	     * after the ^W causes AR8 to change the window name.  It seems
	     * that Window_With_Name() tries to access a BadWindow within
	     * its recursive call.
	     * If this continues to be a problem, a usleep() may make the
	     * problem go away.
	     */
	    success = ! send_command(window_name, *reload_cmds);
	    if (! success && reload_cmds != pdf_viewer.reload_cmds)
	    {
		/*
		 * In some cases the window name might be changed because
		 * of the first command.
		 * For example, in AR7 the first command (^W) "closes"
		 * the document, and the window title changes to reflect
		 * the fact there is no document.  So try that command with
		 * the "plain" window name.
		 */
		success = ! send_command(pdf_viewer.window_name, *reload_cmds);
	    }
	    if (! success)
		break;
	    reload_cmds++;
	}
	if (focus_reset)
	    (void)reset_focus();
	break;

      case USE_FORK_EXEC:
	return 2;  /* TODO: someone who cares can put some code here */
	break;

      default:
	fprintf(stderr, "%s: internal error: invalid reload_method.  Sorry.\n",
		progname);
	return 99;
	break;
    }

    if (success)
	return 0;

    /* 
     * No luck re-loading the file.
     * (Try to) start a new instance of the viewer program.
     */
    if ((pid = fork()) == 0)
    {
	const char * argv_list[2 * MAX_OPTIONS + 3];
	const char ** p;
	int i = 0;

	argv_list[i++] = pdf_viewer.program_name;
	for (p = pdf_viewer.exec_pre_options; *p;)
	    argv_list[i++] = *p++;
	argv_list[i++] = filename;
	for (p = pdf_viewer.exec_post_options; *p;)
	    argv_list[i++] = *p++;
	argv_list[i++] = NULL;
	if (execvp(pdf_viewer.program_name, (char **)argv_list))
	{
	    fprintf(stderr, "%s: %s startup failed\n", progname,
		    pdf_viewer.program_name);
	    return 5;
	}
    }
    else if (pid < 0)
    {
	fprintf(stderr, "%s: fork() failed!?\n", progname);
	return 10;
    }
    
    return 0;
}



int
main(int argc, char * argv[])
{
    const char * viewer = pdf_viewers[0].short_name;
    char * env_viewer;
    int i;
    int viewer_index = -1;
    int focus_reset = 0;

    progname = argv[0];

    env_viewer = getenv("PDF_VIEWER");
    if (env_viewer != NULL)
	viewer = env_viewer;

    if (argc < 2 || !strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))
    {
	usage();
	return argc < 2 ? EXIT_FAILURE : EXIT_SUCCESS;
    }

    if (!strcmp(argv[1], "-v") || !strcmp(argv[1], "--version"))
    {
	printf("This is pdfopen version %s\n", VERSION);
	return EXIT_SUCCESS;
    }

    if (!strcmp(argv[1], "--reset_focus") || !strcmp(argv[1], "-r"))
    {
	argc--;
	argv++;
	focus_reset = 1;
    }

    if (argc == 4 && !strcmp(argv[1], "-viewer"))
    {
	viewer = argv[2];
	argc -= 2;
	argv += 2;
    }

    if (argc != 2)
    {
	fprintf(stderr, "%s: invalid number of arguments\n", progname);
	usage();
	return EXIT_FAILURE;
    }

    for (i = 0; i < sizeof(pdf_viewers) / sizeof(pdf_viewers[0]); i++)
    {
	if (!strcmp(pdf_viewers[i].short_name, viewer))
	{
	    viewer_index = i;
	    break;
	}
    }

    if (viewer_index < 0)
    {
	fprintf(stderr, "%s: Unknown PDF viewer '%s'\n", progname, viewer);
	usage();

	return EXIT_FAILURE;
    }

    if (view_file(argv[1], viewer_index, focus_reset))
	return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
