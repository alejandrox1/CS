#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>


// get_error returns error message.
char* get_error(int n)
{
	size_t size = 1024;
	char *buff = (char *) malloc(size);
	if (buff == NULL)
		return NULL;

	// TODO: better error handling here... 
	if (strerror_r(n, buff, size) < 0)
		return NULL;
	return buff;
}


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
		fprintf(stderr, "Error getting socket error code: %s\n", get_error(status));
		return -1;
	}
	if (err != 0)
	{
		fprintf(stdout, "socket error: %s\n", get_error(err));
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
