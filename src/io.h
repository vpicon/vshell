#ifndef _VSHELL_IO_H
#define _VSHELL_IO_H


#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


#include "defs.h"

void init_command_io(command_type *command);
void set_command_io(command_type *command, 
                    char *const filename, enum io_type t);



#endif  // _VSHELL_IO_H
