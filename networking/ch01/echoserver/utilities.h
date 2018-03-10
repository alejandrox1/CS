#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define ERRSIZE 4096


// check_socket is used to check whether a client is still connected and to
// clear out any errors before shutting down. 
int check_socket(int socket_fd)
{
	int err = 0;
	socklen_t len = sizeof(err);
	int status;

	status = getsockopt(socket_fd, SOL_SOCKET, SO_ERROR, &err, &len);
	if (status != 0)
	{
		char errmsg[ERRSIZE];
		strerror_r(errno, errmsg, ERRSIZE);
		fprintf(stderr, "Error getting socket error code: %s\n", errmsg);
		return -1;
	}
	if (err != 0)
	{
		char errmsg[ERRSIZE];
		strerror_r(err, errmsg, ERRSIZE);
		fprintf(stdout, "socket error: %s\n", errmsg);
		return 0;
	}
	return 1;
}


// close_socket will gracefully shut down and close a socket.
void close_socket(int comm_fd)
{
	int status;
	if (comm_fd > 0)
	{
		// Clear errors to prevent close from failing.
		check_socket(comm_fd);

		status = shutdown(comm_fd, SHUT_RDWR);
		if (status < 0)
			if (errno != ENOTCONN && errno != EINVAL)
				perror("shutting down client socket");
		
		status = close(comm_fd);
		if (status < 0)
			perror("closing client socket");
	}
}



#endif // __UTILITIES_H__
