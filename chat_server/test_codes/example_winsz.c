#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void move_cursor_to_right(int x) {
	printf("\033[%dC", x);
}

int get_term_size() {
	struct winsize ws;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
	return ws.ws_col;
}

void print_right(char *s) {
	int term_size = get_term_size();
	int str_len = strlen(s);

	printf("term_size = %d, str_len = %d\n", term_size, str_len);

	move_cursor_to_right(term_size - str_len);
	printf("%s\n", s);
}

int main(void) {
	int termsize = get_term_size();
	char *test_msg = "this msg len is 18";

	print_right(test_msg);

	return 0;
}
