#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include "errutilities.h"
#include "fileutilities.h"
#include "pthrutils.h"
#include "serverops.h"
#include "tcputilities.h"


#define NUMTHREADS 2
#define CONNPORT "22000"
#define CONNMAX 1000
#define CONNBACKLOG 1000
#define ERRMSG 1024
#define NFILES 1000


int listenfd;
char filenames[NFILES][FNAMESIZE];
void startServer();
void* handler(void*);
void init()
{
	srand(time(NULL));

}

int main()
{
	/* SERVER STARTUP */
	init();

	/* GENERATE FILES */
	clock_t t = clock();
	createNFiles(NFILES);
	t = clock() - t;
	fprintf(stdout, "Time taken to create files %f\n", ((double)t)/CLOCKS_PER_SEC);
	listFiles(filenames);

	/* CONFIGURE SERVER */
	startServer();

	/* ACCEPT CONNECTIONS */
	int status, tid = 0;
	char errmsg[ERRMSG];                                                        
	struct sockaddr_in clientaddr;                                              
	socklen_t addrlen;
	pthread_t threads[NUMTHREADS];                                                         
    params_t params;
	pthread_mutex_init (&params.mutex, NULL);                                   
	pthread_cond_init (&params.done, NULL);
	pthread_mutex_lock(&params.mutex);
	while (1)
	{
		// System call extracts the 1st connection request on queue of pending
		// connections (CONNBACKLOG) for listening socket and creates a new
		// connected socket, returning a file descriptor refereing to that 
		// socket.
		addrlen = sizeof(clientaddr);
		int commfd = accept(listenfd, (struct sockaddr *)&clientaddr, &addrlen);
		// Error accepting connection.
		if (commfd < 0)
		{	
			getError(errno, errmsg);
			fprintf(stderr, "Error accepting request. %s\n", errmsg);
			if((errno == ENETDOWN || errno == EPROTO || errno == ENOPROTOOPT || 
						errno == EHOSTDOWN || errno == ENONET || 
						errno == EHOSTUNREACH || errno == EOPNOTSUPP || 
						errno == ENETUNREACH)) 
			{
				continue;
			}
			break;
			
		}
		// Process client reuests.
		else 
		{
			params.commfd = commfd;
			status = pthread_create(&threads[tid], NULL, handler, &params);
			if (status != 0)                                                        
				fprintf(stderr, "Error creating thread. %s\n", errmsg);
			pthread_cond_wait(&params.done, &params.mutex);
			tid = (tid+1) % NUMTHREADS;
		}
	}

	pthread_mutex_destroy(&params.mutex);                                       
	pthread_cond_destroy(&params.done);
	shutdown(listenfd, SHUT_RDWR);
	close(listenfd);

	exit(0);
}



void startServer() 
{
	int status;
	char errmsg[ERRMSG];
	char port[6];
	strcpy(port, CONNPORT);
	struct addrinfo hints, *res, *p;
	
	// getaddrinfo for host.
	bzero(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	status = getaddrinfo(NULL, port, &hints, &res);
	if (status != 0)
	{
		getError(errno, errmsg);
		fprintf(stderr, "Error in getaddrinfo. %s\n", errmsg);
		exit(1);
	}

	// Bind a socket.
	for (p=res; p!=NULL; p=p->ai_next)
	{
		bzero(errmsg, sizeof(errmsg));
		// Creates and endpoint for communication and return a file descriptor.     
		// The first argument specifies the communication domain; selects the 
		// protocol family to be used.                                           
		listenfd = socket(p->ai_family, p->ai_socktype, 0);                           
		if (listenfd < 0)                                                          
		{                                                   
			getError(errno, errmsg);
			fprintf(stderr, "Error creating a socket. %s\n", errmsg);      
			continue;                        
		}
		// Bind a name to a socket. Assign the address specified by servaddr to 
		// socket refered by file descriptor listenfd.                                                    
		status = bind(listenfd, p->ai_addr, p->ai_addrlen);  
		if (status == 0)
			break;
	}
	if (p==NULL)
	{
		getError(errno, errmsg);
		fprintf(stderr, "Error creating or binding address to socket. %s\n", errmsg);
		exit(1);             
	}

	// Free the dynamically allocated linked list res.
	freeaddrinfo(res);

	// Listen for connections on a socket.
	// Mark the socket lsiten_fd as a passive socket (accept incoming 
	// connection requests).                                                           
	status = listen(listenfd, CONNBACKLOG);
	if (status < 0)
	{
		getError(errno, errmsg);
		fprintf(stderr, "Error while marking listenfd as passive socket. %s\n", errmsg);
		exit(-1);
	}
}


void* handler(void* args)
{
	int i, commfd;
	pthread_mutex_lock(&(*(params_t*)(args)).mutex);                                                                          
	commfd = (*(params_t*)(args)).commfd;                                         
	pthread_mutex_unlock(&(*(params_t*)(args)).mutex);                           
	pthread_cond_signal(&(*(params_t*)(args)).done);

	for (i=0; i<NFILES; i++)                                               
		serverSendFile(commfd, filenames[i]); 

	closeSocket(commfd);
	return NULL;
}
