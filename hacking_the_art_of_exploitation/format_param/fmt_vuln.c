#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
	char text[1024];
	static int another_val = 10;
	static int test_val = -72;

	if (argc < 2) {
		printf("使用方法: %s <出力するテキスト>\n", argv[0]);
		exit(0);
	}
	strcpy(text, argv[1]);

	printf("ユーザによって入力されたテキストを出力する正しい方法:\n");
	printf("%s", text);

	printf("\nユーザによって入力されたテキストを出力する誤った方法:\n");
	printf(text);

	printf("\n");

	printf("\t[+] Debug: &test_val = %p, *test_val = %08x, test_val = %d\n", &test_val, test_val, test_val);
	printf("\t[+] Debug: &another_val = %p, *another_val = %08x, another_val = %d\n", &another_val, another_val, another_val);

	exit(0);
}
