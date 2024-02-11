// 文字列から数字を切り出す。

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int tokenizer(char *not_tokenized_str, int *tokenized_str) {
	char *tok;
	int total_elements;


	tok = strtok(not_tokenized_str, " ");
	for (total_elements = 0; tok; ) {
		tokenized_str[total_elements] = atoi(tok);
		tok = strtok(NULL, " ");
		total_elements++;
	}

	return total_elements;
}

int main(void) {
	char str[100];
	int tokenized_str[100];

	fgets(str, sizeof(str), stdin);

	// str(string) -> tokenized_str(int)
	// return number of elemens
	int total_elements = tokenizer(str, tokenized_str);

	for(int i = 0; i < total_elements; i++) {
		printf("%d\n", tokenized_str[i]);
	}

	return 0;
}
