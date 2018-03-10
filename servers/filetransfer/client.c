#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "errutilities.h"
#include "clientops.h"
#include "pthrutils.h"
#include "tcputilities.h"


#define CONN_PORT 22000
#define ERRMSG 1024


int main(int argc, char **argv)
{
	int status;
	char errmsg[ERRMSG];
	int sockfd;
	struct sockaddr_in servaddr;

	bzero(&servaddr, sizeof(servaddr));                                        
	servaddr.sin_family = AF_INET;                                              
	servaddr.sin_port = htons(CONN_PORT);
	inet_pton(AF_INET, "127.0.0.1", &(servaddr.sin_addr));

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		getError(errno, errmsg);
		fprintf(stderr, "Error creating a socket. %s\n", errmsg);           
		exit(1);
	}

	// System call connects socket refered by the file descriptor sockfd to
	// specified address.
	status = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	if (status < 0)
	{
		getError(errno, errmsg);
		fprintf(stderr, "Error connecting. %s\n", errmsg);
		exit(1);
	}

	/* Server entrypoint */
	recvFile(sockfd);
	
	closeSocket(sockfd);
	exit(0);
}

