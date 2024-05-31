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

#define BUFFER_SIZE 2048

static _Atomic int is_connected = 1;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* recv_msg(void* sockfd) {
	char buff_in[BUFFER_SIZE];
	int rlen;
	//printf("in recv_msg: sockfd = %d\n", *(int *)sockfd);
	
	while (is_connected) {
		rlen = read(*(int *)sockfd, buff_in, sizeof(buff_in)-1);

		if (rlen > 0) {
	 		buff_in[rlen] = '\0';
			printf("%s", buff_in);


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
	struct sockaddr_in serv_addr;
	char buff_out[BUFFER_SIZE];
	pthread_t tid;
	char *command;
	size_t msg_len;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		fprintf(stderr, "[+] Error: %s\n", strerror(errno));
		exit(1);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(5000);
	//serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	// 必要?
	signal(SIGPIPE, SIG_IGN);

	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		fprintf(stderr, "%s\n", strerror(errno));
		exit(1);
	}

	/* サーバーからのメッセージ処理担当 */
	pthread_create(&tid, NULL, &recv_msg, &sockfd);

	/* 入力処理担当 */
	printf("<[ Connected to chat server ]>\n");
	while(is_connected) {

		if (fgets(buff_out, sizeof(buff_out), stdin) == NULL) {
			fprintf(stderr, "[+] Error at fgets()\n");
			continue;
		}

		/* \nが残るとstrcmp()が正常に動作しないため、\nを\0にする */
		msg_len = strlen(buff_out);
		if (msg_len > 0 && buff_out[msg_len-1] == '\n') {
			buff_out[msg_len-1] = '\0';
		}

		/* write() が continue することは望ましくない? */
		pthread_mutex_lock(&lock);
		if(is_connected && write(sockfd, buff_out, strlen(buff_out)) < 0){
			fprintf(stderr, "[+] Error at write(): %s\n", strerror(errno));
			is_connected = 0;
			close(sockfd);
		}
		pthread_mutex_unlock(&lock);

	}
	/* while()を抜けるのは、必ず is_connected = 0 にする処理のみ */

	pthread_join(tid, NULL);
	pthread_mutex_destroy(&lock);

	return 0;
}
