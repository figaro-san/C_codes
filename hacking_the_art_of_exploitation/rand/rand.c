#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main(void) {
	int i;
	printf("RAND_MAX: %u\n", RAND_MAX);
	srand(time(0));

	printf("0 ~ RAND_MAX\n");
	for(i = 0; i < 8; i++) {
		printf("%d\n", rand());
	}

	printf("0 ~ 20\n");
	for (i = 0; i < 8; i++) {
		printf("%d\n", (rand()%20)+1);
	}

	return 0;
}
