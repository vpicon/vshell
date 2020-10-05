#ifndef _VSHELL_PARSE_H
#define _VSHELL_PARSE_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defs.h"


char **parse_tokens(char *input);
void clear_tokens(char **command);


#endif  // _VSHELL_PARSE_H
