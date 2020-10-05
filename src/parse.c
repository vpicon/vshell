#include "parse.h"


/**
 * Parse the contents of the nulterminated input
 * char array pointed by input pointer
 * and store the command in SHELL.command.
 * If the input has invalid characters or contains an
 * invalid command, the function returns 2. Otherwise,
 * return 0.
 */
char **parse_tokens(char *input) {
    char **command = NULL;  /* NULL terminated array of strings
                               where each string is a parsed token
                               from the given input. */

    size_t n_tokens = 0;  /* Number of tokens parsed */
    int input_len = (int) strnlen(input, MAX_INPUT_LEN + 1);

    int i_begin = 0;      /* Start position parsed tokens in input */
    size_t token_len = 0; /* Length of the token currently being read */

    for (int i = 0; i <= input_len; i++) {
        if (input[i] == ' ' || input[i] == '\0') { /* Token delimiters */
            if (token_len > 0) {  /* Found end of token */
                n_tokens++;
                command = realloc(command,
                                  n_tokens * sizeof(char*));
                if (command == NULL) { /* realloc failed */
                    perror("realloc");
                    exit(1);
                }
                /* allocate token_len+1 bytes for the token chars
                 * and strndup will add '\0' to the ending byte.
                 */
                char *token = strndup(&input[i_begin], token_len);
                if (token == NULL) { /* strndup error */
                    perror("strndup");
                    exit(1);
                }
                command[n_tokens - 1] = token; /* add new token to array */
            }
            token_len = 0;
        } else {  /* We are inside a token */
            if (token_len == 0) { /* New token found */
                i_begin = i;
            }
            token_len++;
        }
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

    char **command = parse_tokens(argv[1]);

    for (char **str_p = command; *str_p != NULL; str_p++) {
        printf("%s ", *str_p);
    }
    printf("\n");
    clear_tokens(command);

    return 0;
}

#endif
