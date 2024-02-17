#include <stdio.h>
#include <time.h>
#include <sys/time.h>

typedef struct tm tm;

void getdate(tm *current_time) {
	long int seconds_since_epoch;

	// 1970年1月1日からの経過時間
	seconds_since_epoch = time(0);

	// tm構造体へ設定
	localtime_r(&seconds_since_epoch, current_time);
}

double gettime(){
        struct timeval tp;
        double ret;
        gettimeofday(&tp, NULL);
        ret = (double)(tp.tv_sec & 0x00ffffff) + (double)tp.tv_usec / 1000000;
        return ret;
}


int main() {
	double time_start = gettime();

	tm current_time;
	getdate(&current_time);
	printf("現在時刻 = %2d:%2d:%2d\n", current_time.tm_hour, current_time.tm_min, current_time.tm_sec);

	double time_end = gettime();
	printf("処理時間 = %lf\n", time_end - time_start);

	return 0;
}
