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
 * Given a command_type struct pointer and a
 * pipe of filedescriptors (returned by pipe(2))
 * prepares the command input/output and
 * executes the command via a system call to
 * exec(2).
 */
void exec_command(command_type *command, int pipe_fd[]) {
    char *command_name = command->tokens[0];
    close(pipe_fd[IN]);
    if (command->next_command != NULL) {
        command->io[OUT] = pipe_fd[OUT];
    } else {
        close(pipe_fd[OUT]);
    }

    /* Set input/output ends for the execution */
    if (command->io[IN] != STDIN_FILENO)
        dup2(command->io[IN], STDIN_FILENO);
    if (command->io[OUT] != STDOUT_FILENO)
        dup2(command->io[OUT], STDOUT_FILENO);
    clear_command_io(command);

    if(execvp(command_name, command->tokens) == -1) { /* execvp failed */
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
                clear_command_tokens(command);
                clear_command_io(command);
                continue;
            } else { // special_command_flag == 2
                // history;
            }
        }

        while(command != NULL) {
            int pipe_fd[2] = {STDIN_FILENO, STDOUT_FILENO};
            if (pipe(pipe_fd) == -1) {
                perror("pipe");
                exit(1);
            }

            /* Execute commands from parsed input */
            pid_t pid;
            if ((pid = fork()) == -1) {  /* Fork failed */
                perror("fork");
                exit(1);
            }

            /* In the child process */
            if (pid == 0) {
                exec_command(command, pipe_fd);
            }

            /* In the parent (shell) process */
            if(wait(NULL) == -1) {
                perror("waitpid");
                exit(1);
            }

            clear_command_tokens(command);
            clear_command_io(command);
            command = command->next_command;
            /* Set command pipe output */
            if (command != NULL) {
                close(pipe_fd[OUT]);
                command->io[IN] = pipe_fd[IN];
            }
        }
    }

    return 0;
}
