#ifndef __TCPUTILITIES_H__
#define __TCPUTILITIES_H__

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define MIN(x,y) (((x) < (y)) ? (x) : (y))


// checkSocket is used to check whether a client is still connected and to
// clear out any errors before shutting down. 
int checkSocket(int socketfd)
{
	int err = 0;
	char errmsg[ERRMSG];
	socklen_t len = sizeof(err);
	int status;

	status = getsockopt(socketfd, SOL_SOCKET, SO_ERROR, &err, &len);
	if (status != 0)
	{
		getError(status, errmsg);
		fprintf(stderr, "Error getting socket error code: %s\n", errmsg);
		return -1;
	}
	if (err != 0)
	{
		errno = err;
		getError(err, errmsg);
		fprintf(stdout, "socket error: %s\n", errmsg);
		return -1;
	}
	return 0;
}

// getClientIP gets the IP address of client connection to server.
void getClientIP(int commfd, char ip[], size_t size)
{
	char errmsg[ERRMSG];
	struct sockaddr_storage addr;
	socklen_t len = sizeof(addr);
	int status;

	status = getpeername(commfd, (struct sockaddr*)&addr, &len);
	if (status < 0)
	{
		getError(errno, errmsg);
		fprintf(stderr, "Error getting. %s\n", errmsg);
		return;
	}

	if (addr.ss_family == AF_INET)
	{
		struct sockaddr_in *s = (struct sockaddr_in *)&addr;
		inet_ntop(AF_INET, &s->sin_addr, ip, size);
	} 
	else
	{
		struct sockaddr_in6 *s = (struct sockaddr_in6 *)&addr;
		inet_ntop(AF_INET6, &s->sin6_addr, ip, size);
	}
}


// closeSocket will gracefully shut down and close a socket.
void closeSocket(int commfd)
{
	int status;
	if (commfd > 0)
	{
		// Clear errors to prevent close from failing.
		checkSocket(commfd);

		status = shutdown(commfd, SHUT_RDWR);
		if (status < 0)
			if (errno != ENOTCONN && errno != EINVAL)
				perror("shutting down client socket");
		
		status = close(commfd);
		if (status < 0)
			perror("closing client socket");
	}
}



#endif // __TCPUTILITIES_H__
