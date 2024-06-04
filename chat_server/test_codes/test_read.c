#include <stdio.h>
#include <unistd.h>

int main(void) {
	char buff[10];
	read(STDIN_FILENO, buff, sizeof(buff)-1);
	buff[9] = '\0';
	printf("%s\n", buff);
	read(STDIN_FILENO, buff, sizeof(buff)-1);
	buff[9] = '\0';
	printf("%s\n", buff);
}
