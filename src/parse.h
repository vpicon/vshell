#ifndef _VSHELL_PARSE_H
#define _VSHELL_PARSE_H


#include "defs.h"


command_t *parse_command(char *input);
char *get_token(char **str);
void init_command(command_t *command);
void add_command_token(command_t *command, char *token);
void clear_command_tokens(command_t *command);
void clear_tokens(char **tokens);



#endif  // _VSHELL_PARSE_H
