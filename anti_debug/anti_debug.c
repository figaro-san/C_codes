// gdbなどの使用を検知して強制終了させるプログラム
//
// 回避策
// objdump -M intel -d anti_debug
// でバイナリからptrace関数呼び出しを見つける。
// その後hexeditなどでnop命令(90 90 90 90 90)を直接バイナリに書き換えることで回避。

// こちらよりお借りした
// https:zenn.dev/yurayake/articles/68205a2d94366://zenn.dev/yurayake/articles/68205a2d94366f/

#include <stdio.h>
#include <sys/ptrace.h>

int main(void) {
	if (ptrace(PTRACE_TRACEME, 0, 1, 0) == -1) {
		printf("[+] Worning: Debugger detected\n");
		return 1;
	} else {
		printf("OK!\n");
	}

	return 0;
}
