#include <stdio.h>
#include "libpersonal.h"

int main(int argc, char *argv[])
{	
	char *endp;
	int32_t decimal;

	if (argc != 2) 
		exit_with_msg("[+] Usage: ./pb <decimal number>\n");

	decimal = ec_strtol_10(argv[1], endp);

	if (decimal < 0) 
		exit_with_msg("[+] Error: negative number is not supported\n");

	printf("input : %d\n", decimal);

	print_as_binary(decimal);

	return 0;
}
