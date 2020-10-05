#ifndef _VSHELL_DEFS_H
#define _VSHELL_DEFS_H


/*** DEFINES ***/

#define MAX_INPUT_LEN 80  /* Maximum length of each input */

/* Valid characters for filenames and commands are all printable
 * chars for simplicity.
 */
#define VALID_CHAR(c) (32 <= (c) && (c) <= 126) 

/*** GLOBAL VARIABLES ***/

/**
 * Configuration variables of the shell.
 */
struct shell_config {
    char *prompt;                    /* buffer containing shell prompt */
    char input[MAX_INPUT_LEN + 1];   /* buffer containing each command input */
                                     /* add 1 for ending '\0' */
};
struct shell_config SHELL;

/**
 * Global struct variable containing all status information
 * of the shell.
 */
struct shell_status {
    int run;   /* flag to determine when to exit program 
                       * if flag is:
                       *    0: keep running shell
                       *    1: stop execution
                       */

    int input; /* flag determining status of the input read:
                       * if flag is
                       *    0: read succesfully
                       *    1: input too long
                       *    2: input containing non-valid character
                       */
};
struct shell_status STATUS;


#endif  // _VHSELL_DEFS_H
