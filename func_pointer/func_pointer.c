#include <stdio.h>

int func_one() {
	printf("1つ目の関数\n");
	return 1;
}

int main() {
	//関数のアドレスを持つポインタ変数
	int (*function_ptr) ();
	int value;

	// 関数のアドレス渡し
	function_ptr = func_one;
	printf("function_ptr: %p\n", function_ptr);

	// 関数のアドレスから関数を実行
	value = function_ptr();
	printf("戻り値: %d\n", value);

	return 0;
}
