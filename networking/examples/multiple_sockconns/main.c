/*
 * Use select and fd_set to handle multiple socket connections.
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>

#define TRUE 1
#define FALSE 0
#define PORT 8888

int main(int argc, char *argv[])
{
	int opt = TRUE;
	int master_socket, new_socket, max_sd, sd, client_socket[30];
	int max_clients = 30;
	int addrlen, activity, valread, i;
	struct sockaddr_in address;

	char buffer[1025]; 

	// Set of socket descriptors.
	fd_set readfs;

	// message.
	char *message = "ECHO Daemon v1.0 \r\n";

	// Initialize all client sockets.
	for (i=0; i<max_clients; i++)
		client_socket[i] = 0;
	
	// Create a master socket.
	if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Set master socket to allow multiple connections
	if ( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0)
	{
		perror("etsockopt");
		exit(EXIT_FAILURE);
	}

	// Type of socket created.
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// Bind socket to localhost port 8888
	if (bind(master_socket, (struct sockaddr*)&address, sizeof(address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	printf("Listening on port %d\n", PORT);

	// Try to specify a maximum of 3 pending connnections for master socket
	if (listen(master_socket, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	// Accept incoming connections
	addrlen = sizeof(address);
	puts("waiting for onnections...");

	while (TRUE)
	{
		// Clear the socket set.
		FD_ZERO(&readfs);

		// Add master socket to set.
		FD_SET(master_socket, &readfs);
		max_sd = master_socket;

		// Add child to socket set.
		for (i=0; i<max_clients; i++)
		{
			// socket descriptor.
			sd = client_socket[i];

			// If valid socket descriptor then add to ead list.
			if (sd > 0)
				FD_SET(sd, &readfs);

			// Highest file descriptor number, need it for select function.
			if (sd > max_sd)
				max_sd = sd;
		}

		// Wait for an activity on one of the sockets, time is NULL, so wait
		// indefinetely.
		activity = select(max_sd+1, &readfs, NULL, NULL, NULL);

		if ((activity < 0) && (errno!=EINTR))
			printf("select rror");

		// If something happend on the master socket, then its an incoming
		// connection.
		if (FD_ISSET(master_socket, &readfs))
		{
			if ((new_socket = accept(master_socket, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0)
			{
				perror("accept");
				exit(EXIT_FAILURE);
			}
			// Inform user of socket number - used in send and receive
			// commands.
			printf("New connection, socket fd is %d, ip: %s, port: %d\n", new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));

			// Send new gretting message.
			if (send(new_socket, message, strlen(message), 0) != strlen(message) )
				perror("send");

			// Add new socket to array of sockets.
			for (i=0; i<max_clients; i++)
			{
				// If position is empty.
				if (client_socket[i] == 0)
				{
					client_socket[i] = new_socket;
					printf("Adding %d to list of sockets\n", i);
					break;
				}
			}
		}

		// Else its some IO operation on some other socket.
		for (i=0; i<max_clients; i++)
		{
			sd = client_socket[i];

			if (FD_ISSET(sd, &readfs))
			{
				// Check if it was for closing, also read the incoming message
				if ((valread = read(sd, buffer, 1024)) == 0)
				{
					// Somebody disconnected, get details and prints.
					getpeername(sd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
					printf("Host disconnected, ip %s port %d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

					// Close he socket nd mark 0 in list for reuse.
					close( sd );
					client_socket[i] = 0;
				}
				// Echo back message.
				else
				{
					buffer[valread] = '\0';
					send(sd, buffer, strlen(buffer), 0);
				}
			}
		}
	}
	return 0;
}
