#include <malloc.h>

#include "libpersonal.h"

int main(void) {
	int *int_ptr_1 = (int *)malloc(sizeof(int));
	char *char_ptr_1 = (char *)malloc(sizeof(int));
	int *int_ptr_2 = (int *)malloc(sizeof(int));

	// multi_free(number_of_arguments, arg_1, arg_2, ...);
	multi_free(3, int_ptr_1, int_ptr_2, char_ptr_1);

	return 0;
}
