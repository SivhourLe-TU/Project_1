#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    
	int fd = open("example.txt", O_RDONLY);
	if (fd == -1) {
		perror("tucat: open");
		return 1;
	}
	printf("fd = %d\n", fd);
	close(fd);

	return 0;
}