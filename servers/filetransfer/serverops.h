#ifndef __OPERATIONS_H__
#define __OPERATIONS_H__

#include <arpa/inet.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <openssl/md5.h>
#include "errutilities.h"
#include "tcputilities.h"

#if defined(__linux__)                                                          
#   include <endian.h>                                                          
#elif defined(__FreeBSD__) || defined(__NetBSD__)                               
#   include <sys/endian.h>                                                      
#elif defined(__OpenBSD__)                                                      
#   include <sys/types.h>                                                       
#   define be16toh(x) betoh16(x)                                                
#   define be32toh(x) betoh32(x)                                                
#   define be64toh(x) betoh64(x)                                                
#endif


#define BUFFSIZE 1024
#define FNAMESIZE 10


// echoClient is an operation that will respond to a client by echoing back its
// message.
void echoClient(int *sockfd)
{
	char buff[BUFFSIZE];
	char ip[INET6_ADDRSTRLEN];
	int client_status, commfd = *sockfd;      

	bzero(ip, sizeof(ip));
	getClientIP(commfd, ip, INET6_ADDRSTRLEN);

	while (1)                                                           
	{                                                                   
		client_status = checkSocket(commfd);                     
		if (client_status != 0)
		{
			fprintf(stdout, "Connection %s broken\n", ip);
			break;     
		}
                                                                                
		bzero(buff, BUFFSIZE);                                                
		read(commfd, buff, BUFFSIZE);                                  
		fprintf(stdout, "Echoing back: %s", buff);                     
		write(commfd, buff, strlen(buff)+1);                       
	}
}

void createNFiles(int n)
{
	int i;
	char name[FNAMESIZE],                                                       
         ext[] = ".out";                                                        
	
	for (i=0; i<n; i++)
	{
		bzero(name, sizeof(name));
		generateRandomStr(name, ext, FNAMESIZE-sizeof(ext)); 
		createDevUrandomFile(name);     
	}
}


// sendFile sends contents from buf to socket.
int sendFile(int sockfd, const void * buf, int len)
{
	const char *pbuf = (const char *)buf;
	while (len > 0)
	{
		int sent = send(sockfd, pbuf, len, 0);
		if (sent < 1)
		{
			fprintf(stderr, "cannot write to socket\n");
			return -1;
		}

		pbuf += sent;
		len -= sent;
	}
	return 0;
}

// serverSendFile will send the contents of a file over to a socket.
// It utilizes the function sendFile.
void serverSendFile(int sockfd, const char* filename)
{
	FILE *logFile;
	const char lofFileName[] = "server.log";
	unsigned char checksum[MD5_DIGEST_LENGTH];
	MD5_CTX mdContext;
	char buf[BUFFSIZE];
	struct stat s;

	// Log file.
	logFile = fopen(lofFileName, "a");
	if (logFile == NULL)
	{
		fprintf(stderr, "Error opening log file\n");
		return;
	}

	// Locate and open file to send.
	if (stat(filename, &s) < 0)
	{
		fprintf(stderr, "Cannot stat file\n");
		return;
	}
	FILE *file = fopen(filename, "rb");
	if (file == NULL)                                                           
    {                                                                           
        fprintf(stderr, "Error opening file");                                  
        return;                                                                 
    }	
	
	// Send filename to client.
	if (sendFile(sockfd, filename, strlen(filename)+1) != 0)
		fprintf(stderr, "error sending filename to client\n");
	fprintf(logFile, "%s ", filename);

	// Send file size.
	// conventional 32bit call
	// long size = s.st_size;
	// long tmpSize = htonl(size);
	uint64_t size = (uint64_t)s.st_size;
	uint64_t tmpSize = htobe64(s.st_size);
	fprintf(logFile, "%ld ", size);
	if (sendFile(sockfd, &tmpSize, sizeof(tmpSize)) == 0)
	{
		MD5_Init(&mdContext);
		while (size > 0)
		{
			int n = fread(buf, 1, MIN(sizeof(buf), size), file);
			if (n < 1)
			{
				fprintf(stderr, "cannot read from file\n");
				break;
			}
			MD5_Update(&mdContext, buf, n);
			if (sendFile(sockfd, buf, n) < 0)
				break;

			size -= n;
		}
		MD5_Final(checksum, &mdContext);
		int i;
		for (i=0; i<MD5_DIGEST_LENGTH; i++)
			fprintf(logFile, "%02x", checksum[i]);
		fprintf(logFile, "\n");
	}
	fclose(file);
	fclose(logFile);
}


#endif // __OPERATIONS_H__
