#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

void print_usage()
{
        printf("usage:\n");
        printf("        [+] debi <decimal number>\n");
}

void print_as_bit(int decimal) 
{
	// decimal = 8だとすると; log2(8)+1 = 3+1 = 4(bitで表す際の必要桁数)
	
	if (decimal == 0) {
		printf("0\n");
	} else if (decimal < 0) {
		fprintf(stderr, "Input is overflow or negative number\n");
		exit(1);
	} else {
		int bit_digit = log2(decimal) + 1;
		int *ans_bit = malloc(sizeof(int) * bit_digit);

		int cnt = 0;
		while (decimal != 0) {
			ans_bit[cnt] = decimal%2;
			decimal /= 2;
			cnt++;
		}

		for(int i = bit_digit-1; i >= 0; i--) {
			printf("%d", ans_bit[i]);
		}
		puts("");
	}
}

int main(int argc, char *argv[])
{	
	if (argc != 2) {
		fprintf(stderr, "Invalid arguments\n");
		return 1;
	}
	int decimal = atoi(argv[1]);
	printf("input is %d\n", decimal);

	print_as_bit(decimal);

	return 0;
}
