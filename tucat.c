#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    char buf[4096];

    for (int i = 1; i < argc; i++) {
        int fd = open(argv[i], O_RDONLY);

        if (fd == -1) {
            perror("tucat: open");
            return 1;
        }

        ssize_t n;
        while ((n = read(fd, buf, sizeof(buf))) > 0) {
            write(STDOUT_FILENO, buf, n);

        if (n == -1) {
            perror("tucat: read");
            return 1;
        }

        close(fd);
    }

    return 0;
}