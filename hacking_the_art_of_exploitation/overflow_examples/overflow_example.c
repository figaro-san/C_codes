#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
	int value = 5;
	char buffer_one[8], buffer_two[8];

	strcpy(buffer_one, "one");
	strcpy(buffer_two, "two");

	printf("address of value: \t%p\n", &value);
	printf("value of value: \t%d\n", value);
	printf("address of buffer_one: \t%p\n", &buffer_one);
	printf("value of buffer_one: \t%s\n", buffer_one);
	printf("address of buffer_two: \t%p\n", &buffer_two);
	printf("value of buffer_two: \t%s\n", buffer_two);

	if(argc == 2)
		strcpy(buffer_two, argv[1]);
	puts("");

	printf("address of value: \t%p\n", &value);
	printf("value of value: \t%d\n", value);
	printf("address of buffer_one: \t%p\n", &buffer_one);
	printf("value of buffer_one: \t%s\n", buffer_one);
	printf("address of buffer_two: \t%p\n", &buffer_two);
	printf("value of buffer_two: \t%s\n", buffer_two);

	return 0;
}
