#include <stdio.h>
#include <stdarg.h>

// 引数がスタック状に積まれていく
void func(int argnum, ...) {
	va_list ap;

	// スタックの一番上をapが指す
	va_start(ap, argnum);

	// 最初の引数は普通に取リ出す
	printf("引数の数: %d\n", argnum);

	// va_list型の変数, 取り出す引数の型
	// apが指し示す、次の引数を取り出す
	// その後、取り出した引数をapが指し示す
	for (int i = 0; i < argnum; i++) {
		printf("%d個目の引数: %d\n", i+1, va_arg(ap, int));
	}

	// va_startが実行されるたびに、va_endが実行される必要がある
	va_end(ap);

}

int main(void) {
	int arg_1 = 10;
	int arg_2 = 9;
	int arg_3 = 8;

	func(3, arg_1, arg_2, arg_3);

	return 0;
}
