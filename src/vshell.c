#include "vshell.h"


/**
 * Reads input of the command line and stores the result in
 * the shell command buffer for input. If the input is too long
 * to be stored in the SHELL buffer, returns 1; otherwise
 * returns 0.
 */
char **read_input() {
    int status = 0;
    /* Read each line of input and store it in pointer */
    char *line = NULL;
    size_t n = 0;
    if (getline(&line, &n, stdin) == -1) {
        if (feof(stdin)) {
            STATUS.run = 1;
            free(line);
            return NULL;
        }
        else {
            perror("getline");
            exit(1);
        }
    }

    int i = 0;
    while (i <= MAX_INPUT_LEN && line[i] != '\0') {
        if (!(VALID_CHAR(line[i]) || line[i] == '\n')) {
            status = 2;
            break;
        }
        i++;
    }

    char **command = NULL;
    if(status != 2) {
        if (i > MAX_INPUT_LEN) { /* line is longer than SHELL input buffer */
            status = 1;
        } else {                      /* We have line[i] == '\0' */
            if (line[i - 1] == '\n')  /* line may have ending newline */
                line[i - 1] = '\0';
        }

        if (status != 1) {
            command = parse_tokens(line);
        }
    }

    STATUS.input = status;
    free(line);
    return command;
}


/**
 * Initialize all shell global variables and
 * intern status variables.
 */
void init_shell() {
    /* Initialize STATUS */
    STATUS.run = 0;
    STATUS.input = 0;

    /* Initialize SHELL configuration */
    SHELL.prompt = "vsh$ ";
}


int main() {
    init_shell();

    /* Loop reading and executing commands */
    while (STATUS.run == 0) {
        /* Print shell prompt */
        printf("%s", SHELL.prompt);
        fflush(stdout);

        /* Read input and skip to next command if there were any errors */
        char **command = read_input();
        if (command == NULL) {
            continue;
        }
        if (STATUS.input != 0) {      /* Error on input */
            if (STATUS.input == 1) {  /* Input too long */
                fprintf(stderr, "Input too long, no more than %d "
                                "characters accepted.\n", MAX_INPUT_LEN);
            } else if (STATUS.input == 2) {
                fprintf(stderr, "Input contains non-valid characters.\n");
            }
            /* No need to free any memmory in command, since
             * parse_tokens() was never called in these cases.
             */
            continue; /* Go to read next input */
        }

        /* Execute commands from parsed input */
        pid_t pid;
        if ((pid = fork()) == -1) {  /* Fork failed */
            perror("fork");
            exit(1);
        }

        if (pid == 0) {  /* Child process */
            return 0;
        }
        /* In the parent (shell) process */
        wait(NULL);
        clear_tokens(command);
    }
    return 0;
}
