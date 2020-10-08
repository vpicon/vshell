#include "parse.h"



/**
 * Given an input string with all valid characters
 * and with length 0 < input <= MAX_INPUT_LEN
 * returns a pointer to a command_type structure
 * containing the parsed tokens in the **tokens field
 * and parses all file redirections to put the
 * appropiate FILE descriptor pointers in the
 * io field array of the command_type struct.
 *
 * If there was a syntax error, the STATUS flag
 * is set accordingly and a NULL pointer is returned
 * instead.
 */
command_type *parse_command(char *input) {
    command_type *command = malloc(sizeof(command_type));
    if (command == NULL) { /* malloc failed */
        perror("malloc");
        exit(1);
    }
    init_command(command);

    /* Keep track of the first encountered command (in a possible
     * long list of piped commands), which will be returned by the
     * function.
     */
    command_type *first_command = command;

    char *token;
    while ((token = get_token(&input)) != NULL) {
        if (strcmp(token, "|") == 0) {
            /* Add NULL to the end of command->tokens array */
            add_command_token(command, NULL);

            /* Create a new command struct */
            command_type *new_command = malloc(sizeof(command_type));
            if (new_command == NULL) { /* malloc failed */
                perror("malloc");
                exit(1);
            }
            init_command(new_command);

            /* Point old command to new */
            command->next_command = new_command;
            command = new_command;

            /* Go to read next token */
            continue;
        }
        if (strcmp(token, "<") == 0 || strcmp(token, ">") == 0) {
            char *filename = get_token(&input);
            if (filename == NULL) {
                /* Report syntax error in the status struct */
                STATUS.input = 3;
                STATUS.msg = "Missing filename after </> redirection symbol.";
                /* Clean memory and return.
                 *
                 * We need to add NULL to the end of command->tokens
                 * array to call clean_tokens.
                 */
                add_command_token(command, NULL);
                clear_command_tokens(command);
                clear_command_io(command);
                return NULL;
            }
            enum io_type t = (strcmp(token, "<") == 0) ? IN : OUT;
            set_command_io(command, filename, t);
        } else {
            add_command_token(command, token);
        }
    }

    /* Add NULL to the end of command->tokens array */
    add_command_token(command, NULL);

    return first_command;
}


/**
 * Given a pointer to a non empty null terminated array of characters,
 * returns a pointer to an allocated string (nul terminated)
 * of the first encountered token in *str (a token is any sequence
 * of non_space characters), and leaves the str pointer pointing
 * to the first token delimiter (whitespace) encountered.
 * If there is no such token, returns NULL.
 * When the last token is returned, the given pointer points to the
 * '\0' character in *str.
 * Memmory allocated by the function must be freed calling
 * free().
 */
char *get_token(char **str) {
    size_t token_len = 0; /* Size of token to be parsed */

    while (**str == ' ')  /* Go to first non space character */
        (*str)++;

    char* token_ptr = *str; /* Set a pointer to the first token char */
    while(**str != '\0') {
        if (**str == ' ') { /* Reached end of token */
            break;
        }
        (*str)++;
        token_len++;
    }

    if (token_len == 0) { /* No token was found */
        return NULL;
    }

    char *allocated_str = strndup(token_ptr, token_len);
    if (allocated_str == NULL) { /* strndup failed */
        perror("strndup");
        exit(1);
    }
    return allocated_str;
}



/**
 * Routine to set initialization values to a given
 * command_type struct.
 */
void init_command(command_type *command) {
    command->tokens = NULL;
    command->n_tokens = 0;
    init_command_io(command); /* io array as stdin/stdout by default */
    command->next_command = NULL;
}


void add_command_token(command_type *command, char* token) {
    command->tokens = realloc(command->tokens,
                              (command->n_tokens + 1) * sizeof(char*));
    if (command->tokens == NULL) { /* realloc failed */
        perror("realloc");
        exit(1);
    }
    command->tokens[command->n_tokens] = token;

    if (token != NULL)
        command->n_tokens++;
}


/**
 * Clears all memmory allocated during the execution
 * of parse_command, being command the pointer returned
 * by such routine.
 */
void clear_command_tokens(command_type *command) {
    clear_tokens(command->tokens);
}


/**
 * Clears all memmory allocated during the execution
 * of parse_tokens, being tokens the pointer stored in
 * the field of the command_type pointer returned
 * by such routine.
 */
void clear_tokens(char **tokens) {
    /* Free all the strings in the array */
    for (char **str_ptr = tokens; *str_ptr != NULL; str_ptr++) {
        free(*str_ptr);
    }
    /* Free allocated memory to hold the array of pointers */
    free(tokens);
}



#define DEBUG 0
#if DEBUG

int main(int argc, char *argv[]) {
    command_type *command1 = parse_command("echo 3");
    command_type *command2 = parse_command("cat grep");
    command2->io[0] = stderr;
    printf("%p, %p\n", command1->io[0], command2->io[0]);

    return 0;
}

#endif
