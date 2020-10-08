#include "io.h"


void init_command_io(command_type *command) {
    command->io[0] = STDIN_FILENO;
    command->io[1] = STDOUT_FILENO;
}


void set_command_io(command_type *command,
                    char *const filename, enum io_type t) {
    int fd; /* opened file descriptor */
    if (t == IN) {
        int flags = O_RDONLY;
        fd = open(filename, flags);
    } else {
        int flags = O_WRONLY | O_CREAT | O_TRUNC;
        mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
        fd = open(filename, flags, mode);
    }

    if (fd == -1) { /* fopen failed */
        perror("open");
        STATUS.file_io = 1;
        return;
    }
    command->io[t] = fd;
}


void clear_command_io(command_type *command) {
    int fd_in = command->io[IN];
    if (fd_in != STDIN_FILENO)
        close(fd_in);

    int fd_out = command->io[OUT];
    if (fd_out != STDOUT_FILENO)
        close(fd_out);
}
