#include "vshell.h"



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


/**
 * Reads input of the command line and returns a
 * pointer to a command_type structure containing the
 * parsed command input.
 * On an empty input, the function returns the NULL pointer.
 * If en EOF character is found, the function sets the run
 * STATUS flag to 1.
 *
 * ERRORS:
 * If the input contains any invalid character, the
 * STATUS input value is set to 2.
 * If the input is too long, the STATUS input value is
 * set to 1.
 * On any error, the function returns a NULL pointer.
 */
command_type *read_input() {
    STATUS.input = 0;
    /* Read line of input and store it in pointer */
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
        if (!(VALID_CHAR(line[i]) || line[i] == '\n')) { /* Invalid char found */
            STATUS.input = 2;
            STATUS.msg = "Invalid character in input.";
            break;
        }
        i++;
    }

    command_type *command = NULL;
    if(STATUS.input != 2) {
        if (i > MAX_INPUT_LEN) { /* line is longer than SHELL input buffer */
            STATUS.input = 1;
            STATUS.msg = "Input too long.";
        } else {                      /* We have line[i] == '\0' */
            if (line[i - 1] == '\n')  /* line may have ending newline */
                line[i - 1] = '\0';
        }
        /* Check any input error and no empty input */
        if (STATUS.input != 1 && strnlen(line, MAX_INPUT_LEN + 1) != 0) {
            command = parse_command(line);
        }
    }

    free(line);
    return command;
}


/**
 * Returns if the character array pointer by
 * command_name is special for the terminal:
 * exit command or history feature.
 *
 * On exit command returns 1, on history feature
 * returns 2. Otherwise return 0.
 *
 * Assume command_name is a null terminated
 * non empty character array.
 */
int special_command(command_type *command) {
    char *command_name = command->tokens[0];
    if (strcmp(command_name, "exit") == 0) {
        return 1;
    } else if (command_name[0] == '!') {
        return 2;
    }
    return 0;
}

/**
 * Executes via a system call the given
 * command represented in the command_type
 * structure.
 */
void exec_command(command_type *command) {
    char *file = command->tokens[0];

    /* Set input/output ends for the execution */
    if (command->io[IN] != STDIN_FILENO)
        dup2(command->io[IN], STDIN_FILENO);
    if (command->io[OUT] != STDOUT_FILENO)
        dup2(command->io[OUT], STDOUT_FILENO);
    clear_command_io(command);

    if(execvp(file, command->tokens) == -1) { /* execvp failed */
        perror("execvp");
    }
    return;
}



int main() {
    init_shell();

    /* Loop reading and executing commands */
    while (STATUS.run == 0) {
        /* Print shell prompt */
        printf("%s", SHELL.prompt);
        fflush(stdout);

        /* Read input and skip to next command if there were any errors */
        command_type *command = read_input();
        if (command == NULL) {
            continue;
        }
        if (STATUS.input != 0) {  /* Error on input */
            /* No need to free any memmory, since
             * on input error all memmory in already freed.
             */
            fprintf(stderr, "Error on input: %s\n", STATUS.msg);
            continue; /* Go to read next input */
        }

        int special_command_flag = special_command(command);
        if (special_command_flag != 0) {
            if (special_command_flag == 1) {
                /* Exit in next iteration */
                STATUS.run = 1;
                clear_command(command);
                continue;
            } else { // special_command_flag == 2
                // history;
            }
        }


        /* Execute commands from parsed input */
        pid_t pid;
        if ((pid = fork()) == -1) {  /* Fork failed */
            perror("fork");
            exit(1);
        }

        if (pid == 0) {  /* Child process */
            exec_command(command);
        }
        /* In the parent (shell) process */
        wait(NULL);
        clear_command(command);
        clear_command_io(command);
    }
    return 0;
}
