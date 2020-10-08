#ifndef _VSHELL_PARSE_H
#define _VSHELL_PARSE_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "io.h"


command_type *parse_command(char *input);
char **parse_tokens(char *input);
char *get_token(char **str);
void clear_command(command_type *command);
void clear_tokens(char **tokens);



#endif  // _VSHELL_PARSE_H
