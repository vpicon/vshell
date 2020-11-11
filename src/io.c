#include "io.h"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


/**
 * Given a pointer to a new allocated command_t
 * structure, sets the io field array to the
 * standard input and output file descriptors.
 */
void init_command_io(command_t *command) {
    command->io[0] = STDIN_FILENO;
    command->io[1] = STDOUT_FILENO;
}


/**
 * Given a pointer to a command_t structure,
 * a nulterminated array character representing a file with
 * name filename in the current directory, and an io_t enum t
 * (IN/OUT); opens such file in with the given mode t, and
 * adds the opened file to the io field of the given command.
 */
void set_command_io(command_t *command,
                    char *const filename, enum io_t t) {
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


/**
 * Given a pointer to a command_t structure,
 * closes any open file descriptor that such command
 * has stored in its io array field (if those are
 * different form standard io file descriptors).
 */
void clear_command_io(command_t *command) {
    int fd_in = command->io[IN];
    if (fd_in != STDIN_FILENO)
        close(fd_in);

    int fd_out = command->io[OUT];
    if (fd_out != STDOUT_FILENO)
        close(fd_out);
}
