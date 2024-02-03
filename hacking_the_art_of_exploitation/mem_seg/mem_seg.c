#include <stdio.h>
#include <malloc.h>

int global_var;
int global_initialized_var = 5;

int main(void) {
	int stack_var;
	static int static_var;
	static int static_initialized_var = 5;
	int *heap_var = (int *)malloc(sizeof(int));

	// data seg
	printf("global_initialized_var: %p\n", &global_initialized_var);
	printf("static_initialized_var: %p\n", &static_initialized_var);

	// bss seg
	printf("static_var: %p\n", &static_var);
	printf("global_var: %p\n", &global_var);

	// heap seg
	printf("heap_var: %p\n", &heap_var);

	// stack seg
	printf("stack_var: %p\n", &stack_var);

	return 0;
}
