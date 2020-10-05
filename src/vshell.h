#ifndef _VSHELL_H
#define _VSHELL_H

/*** INCLUDES ***/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
 
#include <unistd.h>

#include "defs.h"
#include "parse.h"


/*** DECLARATIONS  ***/

char **read_input();
void init_shell();


#endif  // _VSHELL_H
