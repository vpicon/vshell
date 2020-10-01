#ifndef _VSHELL_DEFS_H
#define _VSHELL_DEFS_H


/*** DEFINES ***/

#define MAX_INPUT_LEN 80  /* Maximum length of each input */
#define MAX_PROMPT_LEN 20  /* Maximum length of display prompt */


/*** GLOBAL VARIABLES ***/

/**
 * Configuration variables of the shell.
 */
struct shell_config {
    char prompt[MAX_PROMPT_LEN];  /* buffer containing shell prompt */
};

struct shell_config SHELL;

/**
 * Global struct variable containing all status information
 * of the shell.
 */
struct shell_status {
    int run_status; /* flag to determine when to exit program */
};

struct shell_status STATUS;


#endif  // _VHSELL_DEFS_H
