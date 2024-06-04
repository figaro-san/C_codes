#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
	char buff[1024];
	int cnt = 0;

	set_nonblocking(STDIN_FILENO);

	while (1) {
		fgets(buff, sizeof(buff), stdin);
		printf("cnt = %d\n", cnt);
		cnt++;
		sleep(1);
	}

	return 0;

}
