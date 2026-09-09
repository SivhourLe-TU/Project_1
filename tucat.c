/*
./tucat f1.txt f2.txt > combined.txt

./tucat f1.txt f2.txt >> appended.txt

./tucat f1.txt 2> errors.txt

./tucat f1.txt > /dev/null

*/ 

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

    char buff[4096];

    if (argc ==1){
        ssize_t n;

        while((n = read(STDIN_FILENO, buff, sizeof(buff))) > 0) {
            ssize_t written = write(STDOUT_FILENO, buff, n);
            if (written == -1 || written < n) {
                perror("tucat: write fail");
                return 1;
            }
        }

        if (n == -1) {
            perror("tucat: read"); //read fail

            return 1;
        }
    } else {
        for (int i =1; i < argc; i++) {
            int fd = open(argv[i], O_RDONLY);

            if (fd == -1) {
                perror("tucat: open fail"); 
                return 1;
            }

            ssize_t n;
            while((n = read(fd, buff, sizeof(buff))) > 0) {
                write(STDOUT_FILENO, buff, n);
            }

            if (n == -1) {
                perror("tucat: read fail"); 
                return 1;
            }
            close(fd);
            if (close(fd) == -1){
                perror("tucat: close fail");
                return 1;
            }
        }
        
    }
    return 0;
}       