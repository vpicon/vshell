#include "io.h"

void set_command_io(command_type *command,
                    char *const filename, enum io_type t) {
    char *mode = (t == IN) ? "r" : "w";
    FILE *file = fopen(filename, mode);
    if (file == NULL) { /* fopen failed */
        perror("fopen");
        STATUS.file_io = 1;
        return;
    }
    command->io[t] = file;
}
