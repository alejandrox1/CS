#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define BUFFSIZE 1024
#define CONN_PORT 22000


int main(int argc, char **argv)
{
	int sockfd;
	char sendline[BUFFSIZE];
	char recvline[BUFFSIZE]; 
	struct sockaddr_in servaddr;

	bzero(&servaddr, sizeof(servaddr));                                        
	servaddr.sin_family = AF_INET;                                              
	servaddr.sin_port = htons(CONN_PORT);
	inet_pton(AF_INET, "127.0.0.1", &(servaddr.sin_addr));

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	// System call connects socket refered by the file descriptor sockfd to
	// specified address.
	connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	while (1)
	{
		bzero(sendline, BUFFSIZE);
		bzero(recvline, BUFFSIZE);
		fgets(sendline, BUFFSIZE, stdin);

		write(sockfd, sendline, strlen(sendline)+1);
		read(sockfd, recvline, BUFFSIZE);
		printf("recv - %s\n", recvline);
	}

	close(sockfd);
	free(sendline);
	free(recvline);
	exit(0);
}
