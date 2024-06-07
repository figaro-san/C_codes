/* this client program is unfinished */

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "chat.h"


static _Atomic int is_connected = 1;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

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

	move_cursor_to_right(term_size - str_len);
	printf("%s\n", s);
}

int set_nonblocking(int fd) {
	/* fdにセットされている設定フラグを取り出す */
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1) {
		return EXIT_FAILURE;
	}

	/* フラグのO_NONBLOCKをorで1にする */
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
		return EXIT_FAILURE;
	}
	
	

	return EXIT_FAILURE;
}

void* recv_msg(void* sockfd) {
	char buff_in[BUFFER_SIZE];
	char buff_tmp[BUFFER_SIZE];
	int tmp_len = 0;
	int rlen;
	
	while (is_connected) {
		memset(buff_in, 0, BUFFER_SIZE);
		rlen = read(*(int *)sockfd, buff_in, sizeof(buff_in)-1);

		if (rlen > 0) {
	 		buff_in[rlen] = '\0';

			int start = 0;
			for (int i = 0; i < rlen; i++) {
				if (buff_in[i] == '\r' && buff_in[i+1] == '\n') {
					strncpy(buff_tmp + tmp_len, buff_in + start, i - start);
					buff_tmp[tmp_len + i - start] = '\0';

					if (buff_tmp[0] == '[') {
						print_right(buff_tmp);
					} else {
						printf("%s\n", buff_tmp);
					}

					start = i+2;
					tmp_len = 0;
					i++;
				}
			}

			if (start < rlen) {
				strncpy(buff_tmp + tmp_len, buff_in + start, rlen - start);
				tmp_len += rlen - start;
			}


		} else if (rlen == 0) {
			printf("Connection closed by foreign host.\n");

			pthread_mutex_lock(&lock);
			is_connected = 0;
			pthread_mutex_unlock(&lock);

			close(*(int *)sockfd);
			break;

		} else {
			fprintf(stderr, "[+] Error at read(): %s\n", strerror(errno));

			pthread_mutex_lock(&lock);
			is_connected = 0;
			pthread_mutex_unlock(&lock);

			close(*(int *)sockfd);
			break;
		} 
	}


	return NULL;
}

int main(void) {
	int sockfd;
	int s_retval;
	char buff_out[BUFFER_SIZE/2];
	char *command;
	struct sockaddr_in serv_addr;
	struct timeval tv;
	pthread_t tid;
	fd_set rfds;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		fprintf(stderr, "[+] Error at socket(): %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(5000);
	//serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	// 必要?
	signal(SIGPIPE, SIG_IGN);

	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		fprintf(stderr, "[+] Error at connect(): %s\n", strerror(errno));
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	/* サーバーからのメッセージ処理担当 */
	pthread_create(&tid, NULL, &recv_msg, &sockfd);

	/* stdinを非ブロッキングにする */
	if (set_nonblocking(STDIN_FILENO) == EXIT_FAILURE) {
		fprintf(stderr, "Error at set_nonblocking(): %s\n", strerror(errno));
		close(sockfd);
		exit(EXIT_FAILURE);
	} 

	/* 入力処理担当 */
	printf("<[ Connected to chat server ]>\n");
	while(is_connected) {
		/* stdinの監視、タイムアウトは0.1秒 */
		FD_ZERO(&rfds);
		FD_SET(0, &rfds);
		tv.tv_sec = 0;
		tv.tv_usec = 100;

		s_retval = select(1, &rfds, NULL, NULL, &tv);
		if (s_retval == -1) {
			fprintf(stderr, "[+] Error at select(): %s\n", strerror(errno));
			is_connected = 0;
			close(sockfd);
			exit(EXIT_FAILURE);
		} 

		/* stdinにデータが来たときのみ実行する */
		if (FD_ISSET(STDIN_FILENO, &rfds)) {
			if (fgets(buff_out, sizeof(buff_out), stdin) == NULL) {
				fprintf(stderr, "[+] Error at fgets()\n");
				is_connected = 0;
				close(sockfd);
				exit(EXIT_FAILURE);

			}

			pthread_mutex_lock(&lock);
			if(is_connected && write(sockfd, buff_out, strlen(buff_out)) < 0){
				fprintf(stderr, "[+] Error at write(): %s\n", strerror(errno));
				is_connected = 0;
				close(sockfd);
				exit(EXIT_FAILURE);
			}
			pthread_mutex_unlock(&lock);
		}


	}

	pthread_join(tid, NULL);
	pthread_mutex_destroy(&lock);

	return EXIT_SUCCESS;
}
