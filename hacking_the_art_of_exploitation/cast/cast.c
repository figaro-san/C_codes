#include <stdio.h>

// char型のポインタとして定義し、ポインタに1加算するごとに、1バイトずつint_arrの内容を表示させている
// intは4バイトなのでint_arrには値がこのように格納されている。 0x0001, 0x0002, 0x0003, ...
// リトルエンディアンなので、このように表示される。 1, 0, 0, 0, 2, ...

int main(void) {
	int int_arr[5] = {1, 2, 3, 4, 5};

	// charポインタで、1バイトずつ表示させる
	char *char_ptr;
	char_ptr = (char *)int_arr;
	for (int i = 0; i < 5; i++) {
		printf("%d\n", *char_ptr);
		char_ptr++;
	}

	// voidポインタで正常なアドレスの計算を行わせる
	void *void_ptr;
	void_ptr = (void *)int_arr;
	for (int i = 0; i < 5; i++) {
		printf("%d\n", *((int *)(void_ptr)));
		void_ptr = (void *)((int *)void_ptr + 1);
	}

	return 0;
}
