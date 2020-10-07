#ifndef _VSHELL_DEFS_H
#define _VSHELL_DEFS_H

#include <stdio.h> 

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
    int run;     /* flag to determine when to exit program 
                  * if flag is:
                  *    0: keep running shell
                  *    1: stop execution
                  */

    int input;   /* flag determining status of the input read:
                  * if flag is
                  *    0: read succesfully
                  *    1: input too long
                  *    2: input containing non-valid character
                  *    3: invalid input syntax
                  */

    int file_io; /* flag determining status of file opening,
                  * if flag is
                  *    0: file open succesful
                  *    1: failed file open
                  *       
                  * Can be indexed with enum io_type:
                  * file_io[IN] or file_io[OUT]
                  */

    char *msg;
};
struct shell_status STATUS;

/*** TYPES ***/

typedef struct command_type {
    char **tokens;  /* null-terminated array of pointers to strings */
    FILE *io[2];     /* represents input and outpus streams of the command */
} command_type;


enum io_type {
    IN = 0, 
    OUT
};


#endif  // _VHSELL_DEFS_H
