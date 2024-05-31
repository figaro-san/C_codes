#include <stdio.h>
#include <string.h>

int main(void) {
	char buff[3] = {0, 0xb,0xb};
	int rlen = 0;

	fgets(buff, sizeof(buff), stdin);
	rlen = strlen(buff);
	printf("rlen = %d\n", rlen);
	for (int i = 0; i < 3; i++) {
		printf("0x%x\n", buff[i]);

	}
	return 0;
}
