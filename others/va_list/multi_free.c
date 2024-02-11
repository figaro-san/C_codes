// 可変長の引数を受け取る関数と、スタックされた引数を扱う方法を、
// 複数個をfree()するコードとして改造

#include <stdio.h>
#include <stdarg.h>
#include <malloc.h>

void multi_free(int argnum, ...) {
	va_list ap;

	va_start(ap, argnum);

	for (int i = 0; i < argnum; i++) {
		free(va_arg(ap, int*));
	}

	va_end(ap);
}

int main(void) {
	int *int_ptr_1 = (int *)malloc(sizeof(int));
	int *int_ptr_2 = (int *)malloc(sizeof(int));
	int *int_ptr_3 = (int *)malloc(sizeof(int));

	// multi_free(number_of_arguments, arg_1, arg_2, ...);
	multi_free(3, int_ptr_1, int_ptr_2, int_ptr_3);
	
	return 0;
}
