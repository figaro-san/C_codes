#include "libpersonal.h"
#include <stdio.h>

int main(void) {
	short s = 123;
	short ns = -123;
	int i = 0;

	i = (int) s;
	show_int(i);
	show_short(s);

	i = (int) ns;
	show_int(i);
	show_short(ns);

	return 0;
}
