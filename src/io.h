#ifndef _VSHELL_IO_H
#define _VSHELL_IO_H


#include "defs.h"


void init_command_io(command_t *command);
void set_command_io(command_t *command, 
                    char *const filename, enum io_t t);
void clear_command_io(command_t *command);



#endif  // _VSHELL_IO_H
