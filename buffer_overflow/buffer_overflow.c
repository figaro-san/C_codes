#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
	int value = 5;
	char buffer_one[8], buffer_two[8];

	strcpy(buffer_one, "one");
	strcpy(buffer_two, "two");

	printf("address of value: %p\n", &value);
	printf("value of value: %d\n", value);
	printf("address of buffer_one: %p\n", &buffer_one);
	printf("value of buffer_one: %s\n", buffer_one);
	printf("address of buffer_two: %p\n", &buffer_two);
	printf("value of buffer_two: %s\n", buffer_two);

	if(argc == 2)
		strcpy(buffer_two, argv[1]);
	puts("");

	printf("address of value: %p\n", &value);
	printf("value of value: %d\n", value);
	printf("address of buffer_one: %p\n", &buffer_one);
	printf("value of buffer_one: %s\n", buffer_one);
	printf("address of buffer_two: %p\n", &buffer_two);
	printf("value of buffer_two: %s\n", buffer_two);

	return 0;
}
