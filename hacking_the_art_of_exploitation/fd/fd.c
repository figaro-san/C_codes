#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	int fd;
	char *buffer = (char *)malloc(sizeof(char)*100);
	char *datafile = (char *)malloc(sizeof(char)*20);
	strcpy(datafile, "./note");

	if(argc < 2) 
		return -1;
	strcpy(buffer, argv[1]);
	strncat(buffer, "\n", 1);

	/* 
	 * 書き込み専用
	 * ファイルが無いなら新たに作成する
	 * ファイルの末尾にデータを書き足していく
	 * ユーザに対してファイルの読み込みを許可
	 * ユーザーに対してファイルの書き込み許可
	 */
	fd = open(datafile, O_WRONLY | O_CREAT | O_APPEND | S_IRUSR | S_IWUSR);
	if (fd == -1)
		return -1;

	if(write(fd, buffer, strlen(buffer)) == -1)
		return -1;

	if(close(fd) == -1)
		return -1;

	free(buffer);
	free(datafile);

	return 0;
}
