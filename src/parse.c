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
    /* Initialize command struct */
    command->tokens = NULL;
    init_command_io(command); /* io array as stdin/stdout by default */

    int n_tokens = 0; /* Number of tokens that will
                          * be allocated for the command_type struct;
                          * that is: </> and its following filenames
                          * will not add up to this count.
                          */
    char *token;
    while ((token = get_token(&input)) != NULL) {
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
                command->tokens = realloc(command->tokens,
                                          (n_tokens + 1) * sizeof(char*));
                if (command->tokens == NULL) { /* realloc failed */
                    perror("realloc");
                    exit(1);
                }
                command->tokens[n_tokens] = NULL;
                clear_tokens(command->tokens);
                clear_command_io(command);
                return NULL;
            }
            enum io_type t = (strcmp(token, "<") == 0) ? IN : OUT;
            set_command_io(command, filename, t);
        } else {
            n_tokens++;
            command->tokens = realloc(command->tokens,
                                      n_tokens * sizeof(char*));
            if (command->tokens == NULL) { /* realloc failed */
                perror("realloc");
                exit(1);
            }
            command->tokens[n_tokens - 1] = token;
        }
    }

    /* Add NULL to the end of command->tokens array */
    command->tokens = realloc(command->tokens,
                              (n_tokens + 1) * sizeof(char*));
    if (command->tokens == NULL) { /* realloc failed */
        perror("realloc");
        exit(1);
    }
    command->tokens[n_tokens] = NULL;

    return command;
}


/**
 * Clears all memmory allocated during the execution
 * of parse_command, being command the pointer returned
 * by such routine.
 */
void clear_command(command_type *command) {
    clear_tokens(command->tokens);
}



/**
 * Parse the contents of the nulterminated input
 * char array pointed by input pointer
 * and return a pointer to an array of pointers of strings.
 * Suppose the input string has 0 < length <= MAX_INPUT_LEN.
 */
char **parse_tokens(char *input) {
    char **command = NULL;  /* NULL terminated array of strings
                               where each string is a parsed token
                               from the given input. */
    size_t n_tokens = 0;  /* Number of tokens parsed */

    char *token;
    while ((token = get_token(&input)) != NULL) {
        n_tokens++;
        command = realloc(command, n_tokens * sizeof(char*));
        command[n_tokens - 1] = token;
    }

    /* Add NULL to the end */
    command = realloc(command, (n_tokens + 1) * sizeof(char*));
    if (command == NULL) {
        perror("realloc");
        exit(1);
    }
    command[n_tokens] = NULL;

    return command;
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
