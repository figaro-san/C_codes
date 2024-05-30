
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
/* this client program is unfinished */

#include <unistd.h>

void recv_message(int sockfd) {

}

int main(void) {
	int sockfd;
	struct sockaddr_in serv_addr;
	char buff_in[2048];
	char buff_out[1024];
	pthread_t tid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		fprintf(stderr, "[+] Error: %s\n", strerror(errno));
		exit(1);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(5000);
	//serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_addr.s_addr = 0;

	signal(SIGPIPE, SIG_IGN);

	

	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		fprintf(stderr, "%s\n", strerror(errno));
		exit(1);
	}

	printf("<[ Connection started ]>\n");


	return 0;
}
