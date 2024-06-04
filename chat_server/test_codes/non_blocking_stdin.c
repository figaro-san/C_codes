#include <sys/select.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

void set_nonblocking(int fd) {
	/* fdにセットされている設定フラグを取り出す */
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1) {
		fprintf(stderr, "Error at set_nonblocking()\n");
		exit(1);
	}

	/* フラグのO_NONBLOCKをorで1にする */
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
		fprintf(stderr, "Error at set_nonblocking()/fcntl()\n");
		exit(1);
	}
}


int main(void) {
	fd_set rfds;
	struct timeval tv;
	int retval;
	char buff[1024];
	int cnt = 0;
	
	// stdinを非ブロッキング化
	set_nonblocking(0);


	while (1) {
		// stdinを監視
		FD_ZERO(&rfds);
		FD_SET(0, &rfds);
		tv.tv_sec = 0;
		tv.tv_usec = 100;
		retval = select(1, &rfds, NULL, NULL, &tv);

		if (retval == -1) {
			perror("select()");
		} 

		if (FD_ISSET(STDIN_FILENO, &rfds)) {
			fgets(buff, sizeof(buff), stdin);
			printf("input = %s\n", buff);
		}

		printf("cnt = %d\n", cnt++);
	}


	return 0;
}

