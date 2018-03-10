#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define BUFF_SIZE (1024*1024)
#define CONN_PORT 22000


int main(int argc, char **argv)
{
	int sockfd;
	char sendline[100]; //*sendline = (char *)malloc(BUFF_SIZE);      /* Allocate 1MB .*/
	char recvline[100]; //*recvline = (char *)malloc(BUFF_SIZE);      /* Allocate 1MB .*/
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
		bzero(sendline, 100);
		bzero(recvline, 100);
		fgets(sendline, 100, stdin);

		write(sockfd, sendline, strlen(sendline)+1);
		read(sockfd, recvline, 100);
		printf("recv - %s\n", recvline);
	}

	close(sockfd);
	free(sendline);
	free(recvline);
	exit(0);
}
