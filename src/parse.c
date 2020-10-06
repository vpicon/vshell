#include "parse.h"


/* command_type *parse_command(char *input) { */
    /* command_type *command = malloc(sizeof(command_type)); */
    /* if (command == NULL) { [> malloc failed <] */
        /* perror("malloc"); */
        /* exit(1); */
    /* } */

    /* return command; */
/* } */


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
 * of parse_tokens, being command the pointer returned
 * by such routine.
 */
void clear_tokens(char **command) {
    for (char **str_ptr = command; *str_ptr != NULL; str_ptr++) {
        free(*str_ptr);
    }
    free(command);
}



#define DEBUG 0
#if DEBUG

int main(int argc, char *argv[]) {
    if (argc > 2) {
        fprintf(stderr, "usage: %s <string>", argv[0]);
        exit(1);
    }

    char *input = strdup(argv[1]);

    char *token;
    while ((token = get_token(&input)) != NULL)
        printf("%s\n", token);

    return 0;
}

#endif
