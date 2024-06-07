#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

int is_fd_open(int fd) {
	return fcntl(fd, F_GETFD) != -1 || errno != EBADF;
}

int main(void) {
	printf("%d\n", fcntl(0, F_GETFD));

	if (is_fd_open(STDIN_FILENO)) {
		printf("stdin is open\n");
	} else {
		printf("stdin is not open\n");
	}

	return 0;
} 
