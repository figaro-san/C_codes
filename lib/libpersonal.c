#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <malloc.h>

#include "libpersonal.h"

void exit_with_msg(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);

	exit(1);
}

void print_as_binary(int32_t decimal) 
{
	size_t i, cnt = 0;
	
	if (decimal == 0) {
		printf("0\n");

	} else {
		/* bit_widthが格納するのはビット幅で、最大でも32しか格納しない*/
		/* つまりshortでもいい? */
		/* decimal = 8だとすると; log2(8)+1 = 3+1 = 4(bitで表す際の必要桁数) */
		int bit_width = log2(decimal) + 1;

		/* ans_binaryは0か1しか保持しないためbool */
		bool *ans_bit = (bool *)malloc(sizeof(bool) * bit_width);

		while (decimal != 0) {
			ans_bit[cnt] = decimal%2;
			decimal /= 2;
			cnt++;
		}

		for(i = bit_width; i > 0; i--) {
			printf("%d", ans_bit[i-1]);
		}
		puts("");
	}

}

int32_t ec_strtol_10(const char *str, char *endp) 
{
	int32_t decimal;
	decimal = strtol(str, &endp, 10);
	if (*endp != '\0') 
		exit_with_msg("[+] Error: \'%s\' is invalid input\n", endp);
	return decimal;
}

void multi_free(uint32_t argnum, ...) 
{
	va_list args;
	va_start(args, argnum);

	for (int i = 0; i < argnum; i++){
		void *ptr = va_arg(args, void*);
		printf("%p\n", ptr);
		free(ptr);
	}
	
	va_end(args);
}
