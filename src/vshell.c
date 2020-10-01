#include "vshell.h"

/**
 * Initialize all shell global variables and
 * intern status variables.
 */
void init_shell() {
    /* Initialize STATUS */
    STATUS.run_status = 1;

    /* Initialize SHELL configuration */
    strncpy(SHELL.prompt, "vsh$", MAX_PROMPT_LEN);
}




int main() {
    init_shell();
    while (STATUS.run_status) {

    }
    return 0;
}
