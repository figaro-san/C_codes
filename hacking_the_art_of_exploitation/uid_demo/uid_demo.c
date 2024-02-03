#include <stdio.h>
#include <unistd.h>

// idコマンドでどのユーザにどのidが振られているか見える

int main(void) {
	printf("実uid:   %d\n", getuid());
	printf("実効uid: %d\n", geteuid());
	return 0;
}
