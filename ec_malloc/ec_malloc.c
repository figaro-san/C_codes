#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

void *ec_malloc(unsigned int size){
	void *ptr;
	ptr = malloc(size);
	if (ptr == NULL) {
		fprintf(stderr, "Error: could not allocate heap memory.\n");
		exit(-1);
	}
	return ptr;
}

int main(void) {
	char *char_ptr = (char *)ec_malloc(50);
	return 0;
} 
