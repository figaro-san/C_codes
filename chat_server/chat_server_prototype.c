#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

/* 接続を受け付ける最大数 */
#define MAX_CLIENTS 100

/* 最大バッファサイズ */
#define BUFFER_SIZE 2048

/* 
 * _Atomic
 * C11で追加された型修飾子
 */
static _Atomic unsigned int cli_count = 0;

/* 接続した順に、クライアントへ11番からのuidを付与する */
static int uid = 0;

/* クライアント情報保持用の構造体とクライアント情報キュー */
typedef struct {
	struct sockaddr_in addr;
	int connfd;
	int uid;
	char name[32];
} client_t;
client_t *clients[MAX_CLIENTS];

pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t topic_mutex = PTHREAD_MUTEX_INITIALIZER;

//static char topic[BUFFER_SIZE/2];
static char topic[BUFFER_SIZE];



void print_client_addr(struct sockaddr_in addr) {
	printf("%d %d %d %d", 
		addr.sin_addr.s_addr & 0xff,
		(addr.sin_addr.s_addr & 0xff00) >> 8,
		(addr.sin_addr.s_addr & 0xff0000) >> 16,
		(addr.sin_addr.s_addr & 0xff000000) >> 24
	);
}



/* クライアントをクライアント情報キューへ追加 */
void queue_add(client_t *cli) {
	/* mutexでロックして同時アクセスを防ぐ */
	pthread_mutex_lock(&clients_mutex);
	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (!clients[i]) {
			clients[i] = cli;
			break;
		}
	}
	/* ロック開放 */
	pthread_mutex_unlock(&clients_mutex);
}



void send_message_all(char *s) {
	pthread_mutex_lock(&clients_mutex);
	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (clients[i]) {
			if (write(clients[i]->connfd, s, strlen(s)) < 0) {
				fprintf(stderr, "[+] Error at send_message_all(): %s\n", strerror(errno));
				break;
			}
		}
	}
	pthread_mutex_unlock(&clients_mutex);
}



/* 接続してきたクライアント send_message_clientと似ているが、こちらは */
void send_message_self(const char *s, int connfd) {
	if (write(connfd, s, strlen(s)) < 0) {
		fprintf(stderr, "[+] Error: %s\n", strerror(errno));
		exit(1);
	}
}



/* \r\nを\0とする関数 */
void strip_newline(char *s) {
	while (*s != '\0') {
		if (*s == '\r' || *s == '\n') {
			*s = '\0';
		}
		s++;
	}
}



char* _strdup(const char *s) {
	size_t size = strlen(s) + 1;
	char *p = malloc(size);
	if (p) {
		memcpy(p, s, size);
	}
	return p;
}



/* 指定したクライアントのみに送信する */
void send_message_client(char *s, int uid) {
	pthread_mutex_lock(&clients_mutex);
	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (clients[i]) {
			if (clients[i]->uid == uid) {
				if (write(clients[i]->connfd, s, strlen(s)) < 0) {
					fprintf(stderr, "[+] Error: %s\n", strerror(errno));
					break;
				}
			}
		}
	}
	pthread_mutex_unlock(&clients_mutex);
}



void send_active_clients(int connfd) {
	char s[64];

	pthread_mutex_lock(&clients_mutex);
	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (clients[i]) {
			sprintf(s, "<< [%d] %s\r\n", clients[i]->uid, clients[i]->name);
			send_message_self(s, connfd);
		}
	}
	pthread_mutex_unlock(&clients_mutex);
}



/* sender以外の全てに送る */
void send_message_all_without_sender(char *s, int uid) {
	pthread_mutex_lock(&clients_mutex);
	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (clients[i]) {
			if (clients[i]->uid != uid) {
				if (write(clients[i]->connfd, s, strlen(s)) < 0) {
					fprintf(stderr, "%s\n", strerror(errno));
					break;
				}
			}
		}
	}
	pthread_mutex_unlock(&clients_mutex);
}




void queue_delete(int uid) {
	pthread_mutex_lock(&clients_mutex);
	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (clients[i]) {
			if (clients[i]->uid == uid) {
				clients[i] = NULL;
				break;
			}
		}
	}
	pthread_mutex_unlock(&clients_mutex);
}




/* クライアントとの通信を操作する */
void* handle_client(void* arg) {
	char buff_out[BUFFER_SIZE];
	//char buff_in[BUFFER_SIZE/2];
	char buff_in[BUFFER_SIZE];
	int rlen;

	cli_count++;
	client_t *cli = (client_t *) arg;

	printf("<< accept ");
	print_client_addr(cli->addr);
	printf(" referenced by %d\n", cli->uid);

	sprintf(buff_out, "<< Your uid is %d\r\n", cli->uid);
	send_message_self(buff_out, cli->connfd);

	sprintf(buff_out, "[System] %s has joind\r\n", cli->name);
	send_message_all_without_sender(buff_out, cli->uid);

	pthread_mutex_lock(&topic_mutex);
	if (strlen(topic)) {
		buff_out[0] = '\0';
		sprintf(buff_out, "<< current topic: %s\r\n", topic);
		send_message_self(buff_out, cli->connfd);
	}
	pthread_mutex_unlock(&topic_mutex);

	send_message_self("<< see /help for assistance\r\n", cli->connfd);

	/* クライアントからの入力を受け取る */
	/* ここで、buff_inのサイズ-1までを受け取るようにしているので、この後のオーバーフローは考慮しなくとも良い? */
	while ((rlen = read(cli->connfd, buff_in, sizeof(buff_in)-1)) > 0) {
		buff_in[rlen] = '\0';
		buff_out[0] = '\0';
		strip_newline(buff_in);

		/* 空バッファを無視 */
		if (!strlen(buff_in)) {
			continue;
		}

		/* 特殊なオプション */
		if (buff_in[0] == '/') {
			char *command, *param;
			command = strtok(buff_in, " ");

			if (!strcmp (command, "/quit")) {
				break;

			} else if (!strcmp(command, "/ping")) {
				send_message_self("[System] pong\r\n", cli->connfd);

			} else if (!strcmp(command, "/topic")) {
				param = strtok(NULL, " ");

				if (param) {
					pthread_mutex_lock(&topic_mutex);
					topic[0] = '\0';
					while (param != NULL) {
						strcat(topic, param);
						strcat(topic, " ");
						param = strtok(NULL, " ");
					}
					pthread_mutex_unlock(&topic_mutex);
					sprintf(buff_out, "[System] topic changed to: %s\r\n", topic);
					send_message_all(buff_out);

				} else {
					send_message_self("<< message cannot be null\r\n", cli->connfd);
				}

			} else if (!strcmp(command, "/nick")) {
				param = strtok(NULL, " ");
				if (param) {
					char *old_name = _strdup(cli->name);
					if (!old_name) {
						fprintf(stderr, "[+] Error: Cannot allocate memory");
						continue;
					}
					strncpy(cli->name, param, sizeof(cli->name));
					cli->name[sizeof(cli->name)-1] = '\0';
					sprintf(buff_out, "[System] %s is now known as %s\r\n", old_name, cli->name);
					free(old_name);
					send_message_all(buff_out);

				} else {
					send_message_self("<< name cannot be null\r\n", cli->connfd);

				}

			} else if (!strcmp(command, "/msg")) {
				param = strtok(NULL, " ");
				if (param) {
					int uid = atoi(param);
					param = strtok(NULL, " ");
					if (param) {
						sprintf(buff_out, "[PM][%s]", cli->name);
						while (param != NULL) {
							strcat(buff_out, " ");
							strcat(buff_out, param);
							param = strtok(NULL, " ");
						}
						strcat(buff_out, "\r\n");
						send_message_client(buff_out, uid);

					} else {
						send_message_self("<< message cannot be null\r\n", cli->connfd);
					}
				} else {
					send_message_self("<< message cannot be null\r\n", cli->connfd);
				}

			} else if (!strcmp(command, "/list")) {
				sprintf(buff_out, "<< %d clients\r\n", cli_count);
				send_message_self(buff_out, cli->connfd);
				send_active_clients(cli->connfd);

			} else if (!strcmp(command, "/help")) {
				strcat(buff_out, "<< /quit                        Quit chatroom\r\n");
				strcat(buff_out, "<< /ping                        Server test\r\n");
				strcat(buff_out, "<< /topic <message>             Set chat topic\r\n");
				strcat(buff_out, "<< /nick <name>                 Change nickname\r\n");
				strcat(buff_out, "<< /msg <reference> <message>   Send private message\r\n");
				strcat(buff_out, "<< /list                        Show active clients\r\n");
				strcat(buff_out, "<< /help                        Show help\r\n");
				send_message_self(buff_out, cli->connfd);

			} else {
				send_message_self("<< unknown command\r\n", cli->connfd);
			}

		} else {
			/* コマンドが指定されていないなら、メッセージとして送信 */
			snprintf(buff_out, sizeof(buff_out), "[%s] %s\r\n", cli->name, buff_in);
			send_message_all_without_sender(buff_out, cli->uid);
		}
	} 

	/* close connection */
	sprintf(buff_out, "[System] %s has left\r\n", cli->name);
	send_message_all(buff_out);
	close(cli->connfd);

	/* キューからクライアントを削除する */
	queue_delete(cli->uid);
	printf("<< quit ");
	print_client_addr(cli->addr);
	printf(" referenced by %d\n", cli->uid);
	free(cli);
	cli_count--;
	pthread_detach(pthread_self());

	return NULL;
}



int main(void) {
	int listenfd, connfd;
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;
	pthread_t tid;

	/* ソケットの作成
	 * AF_INET = ipv4, SOCK_STREAM = tcp 
	*/
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd == -1) {
		fprintf(stderr, "[+] Error at socket(): %s\n", strerror(errno));
		exit(1);
	}

	/* 
	 * 下２つはネットワークバイトオーダー
	 * INADDR_ANYはどれでもの意
	*/
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(5000);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	/* 
	 * コネクションの切れたソケットへの書き込みが発生すると、SIGPIPEでプロセスが強制終了する 
	 * それを防止
	*/
	signal(SIGPIPE, SIG_IGN);

	/* 
	 * ソケットへアドレスとポートをバインドする
	 * sockaddrへのキャストが必要 
	*/
	if (bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		fprintf(stderr, "[+] Error at bind(): %s\n", strerror(errno));
		exit(1);
	}

	/*
	* listenfdを接続待ちソケットとする
	* 10は保留中の接続のキューの最大値
	*/
	if (listen(listenfd, 10) < 0) {
		fprintf(stderr, "[+] Error at listen(): %s\n", strerror(errno));
		exit(1);
	}

	printf("<[ Server started ]>\n");

	/* 接続受付の開始 */
	while (1) {
		/* クライアントに関する情報の格納 */

		/* 
		 * 接続要求待ちのキューから接続要求を取り出し、接続済みソケットを生成。
		 * それを参照するfdを返す
		*/
		socklen_t clilen = sizeof(cli_addr);
		connfd = accept(listenfd, (struct sockaddr *)&cli_addr, &clilen);
		if (connfd < 0) {
			fprintf(stderr, "[+] Error at accept(): %s\n", strerror(errno));
			exit(1);
		}


		/* 接続要求が100件を超えた場合の処理 */
		if ((cli_count+1) == MAX_CLIENTS) {
			printf("<< Max clients reached\n");
			printf("<< reject ");
			print_client_addr(cli_addr);
			printf("\n");
			close(connfd);
			continue;
		}

		/* クライアントの設定 */
		client_t *cli = (client_t *)malloc(sizeof(client_t));
		cli->addr = cli_addr;
		cli->connfd = connfd;
		cli->uid = uid++;
		sprintf(cli->name, "%d", cli->uid);

		queue_add(cli);

		/* スレッドの作成 */
		pthread_create(&tid, NULL, &handle_client, (void *)cli);

		/* CPU休み */
		sleep(1);
	}

	return 0;
}

