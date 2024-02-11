#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int check_auth(char *passwd) {
	int auth_flag = 0;
	char passwd_buf[16];

	strcpy(passwd_buf, passwd);

	if(strcmp(passwd_buf, "brilling") == 0) {
		auth_flag = 1;
	}

	return auth_flag;
}

int main(int argc, char *argv[]) {
	if (argc != 2) 
		return -1;

	if(check_auth(argv[1])) {
		printf("アクセスを許可します。\n");
	} else {
		printf("アクセスが拒否されました。\n");
	}

	return 0;
}
