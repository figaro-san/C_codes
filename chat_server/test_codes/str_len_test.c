#include <stdio.h>
#include <string.h>

int main(void) {
	char *test = "msg\0msg\0";
	printf("msg length = %lo\n", strlen(test));
	return 0;

}
