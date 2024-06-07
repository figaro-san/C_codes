#include <stdio.h>

int main(void) {
	char *str;
	sprintf(str, "hello");
	printf("%s\n", str);

	for (int i = 0; i < 7; i++) {
		printf("%x ", str[i]);
	}
	puts("");

	sprintf(str, "hi");
	printf("%s\n", str);

	for (int i = 0; i < 5; i++) {
		printf("%x ", str[i]);
	}
	puts("");

	return 0;

}
