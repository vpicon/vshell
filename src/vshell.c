#include "vshell.h"

/**
 * Reads input of the command line and stores the result in
 * the shell command buffer for input. If the input is too long
 * to be stored in the SHELL buffer, returns -1; otherwise
 * returns 0.
 */
int read_input() {
    int return_status = 0;
    /* Read each line of input and store it in pointer */
    char *line = NULL;
    size_t n = 0;
    getline(&line, &n, stdin);

    if (strnlen(line, MAX_INPUT_LEN) == MAX_INPUT_LEN) {
        /* We did not find a '\0' in the first MAX_INPUT_LEN
         * characters, thus the read line overflows our
         * input buffer
         */
        return_status = -1;
    } else {
        strncpy(SHELL.input, line, MAX_INPUT_LEN);
    }

    free(line);
    return return_status;
}



/**
 * Initialize all shell global variables and
 * intern status variables.
 */
void init_shell() {
    /* Initialize STATUS */
    STATUS.run_status = 1;

    /* Initialize SHELL configuration */
    strncpy(SHELL.prompt, "vsh$ ", MAX_PROMPT_LEN);
}




int main() {
    init_shell();
    while (STATUS.run_status) {
        printf("%s", SHELL.prompt);
        fflush(stdout);
        read_input();
    }
    return 0;
}
