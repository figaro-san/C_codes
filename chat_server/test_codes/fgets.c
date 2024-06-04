#include <stdio.h>
#include <string.h>

int main(void) {
	char buff[7];
	fgets(buff, sizeof(buff), stdin);
	for (int i = 0; i < 7; i++) {
		printf("%x ", buff[i]);
	}
	puts("");

	printf("%lu\n", strlen(buff));
	/*
	fgets(buff, sizeof(buff), stdin);
	for (int i = 0; i < 5; i++) {
		printf("%x ", buff[i]);
	}
	puts("");
	*/
	return 0;
}
