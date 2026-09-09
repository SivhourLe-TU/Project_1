/* final version 

Shell redirection descriptions

./tucat f1.txt f2.txt > combined.txt
-> The shell first creates combined.txt for writing and duplicates the file descriptor
into fd 1 (stdout). Executes tucat next which calls write(). 
fd 1 now points at a file instead of the terminal. The output of tucat is 
written to combined.txt.

./tucat f1.txt f2.txt >> appended.txt
-> The shell first creates appended.txt for appending and writes to file
instead of overwriting appended.txt. 

./tucat f1.txt 2> errors.txt
-> The shell points fd 2 at errors.txt. fd 1 still points at the terminal
which results in the output of tucat being written to the terminal
and the error messages being written to errors.txt.

./tucat f1.txt > /dev/null
-> The shell points fd 1 at /dev/null, that deletes any output written to it.
No output leads to stderr going to the terminal.

tucat doesn't change, the shell sets up the file descriptors before executing tucat/main()
 */

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

    char buff[4096];

    if (argc == 1) {
        ssize_t n;

        while ((n = read(STDIN_FILENO, buff, sizeof(buff))) > 0) {
            ssize_t written = write(STDOUT_FILENO, buff, n);
            if (written == -1 || written < n) {
                perror("tucat: write fail");
                return 1;
            }
        }

        if (n == -1) {
            perror("tucat: read fail");
            return 1;
        }

    } else {
        for (int i = 1; i < argc; i++) {
            int fd = open(argv[i], O_RDONLY);

            if (fd == -1) {
                perror("tucat: open fail");
                return 1;
            }

            ssize_t n;
            while ((n = read(fd, buff, sizeof(buff))) > 0) {
                ssize_t written = write(STDOUT_FILENO, buff, n);
                if (written == -1 || written < n) {
                    perror("tucat: write fail");
                    return 1;
                }
            }

            if (n == -1) {
                perror("tucat: read fail");
                return 1;
            }

            if (close(fd) == -1) {
                perror("tucat: close fail");
            }
        }
    }

    return 0;
}