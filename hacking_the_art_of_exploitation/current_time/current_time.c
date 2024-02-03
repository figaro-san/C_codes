#include <stdio.h>
#include <time.h>

int main() {
	long int seconds_since_epoch;
	struct tm current_time, *time_ptr;
	int hour, minute, second, day, month, year;

	// 1970年1月1日からの経過時間
	seconds_since_epoch = time(0);
	printf("time() - エポックからの通算秒数: %ld\n", seconds_since_epoch);

	time_ptr = &current_time;

	// tm構造体へ設定
	localtime_r(&seconds_since_epoch, time_ptr);

	// 3種類の構造体アクセス
	hour = current_time.tm_hour;
	minute = time_ptr->tm_min;
	second = *((int *)time_ptr);

	printf("現在時刻は: %02d:%02d:%02d\n", hour, minute, second);
	return 0;
}
