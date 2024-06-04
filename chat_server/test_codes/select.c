#include <sys/select.h>
#include <stdlib.h>
#include <stdio.h>


int main(void) {
	fd_set rfds;
	struct timeval tv;
	int retval;

	// 集合のリセット
	FD_ZERO(&rfds);

	// 集合へstdinを追加
	FD_SET(0, &rfds);

	// 5秒待つ
	tv.tv_sec = 5;
	tv.tv_usec = 0;

	// 
	retval = select(1, &rfds, NULL, NULL, &tv);
	printf("select() done\n");

	if (retval == -1) {
		perror("select()");
	} else if (retval) {
		printf("今データが取得できました。\n");
	} else {
		printf("5秒以内にデータが入力されませんでした。\n");
	}

	return 0;
}
