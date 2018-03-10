#ifndef __CLIENTOPS_H__
#define __CLIENTOPS_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <openssl/md5.h>
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


// sendString will send input from stdin to server.
void sendString(int sockfd)                                                
{                                                                               
    char sendline[BUFFSIZE];                                                         
    char recvline[BUFFSIZE];                                                         
    
	while (1)                                                                   
    {                                                                           
        bzero(sendline, BUFFSIZE);                                                   
        bzero(recvline, BUFFSIZE);

        fgets(sendline, BUFFSIZE, stdin);                                                                                                                    
        write(sockfd, sendline, strlen(sendline)+1);                            
        read(sockfd, recvline, BUFFSIZE);                                            
        
		printf("received: %s\n", recvline);                                        
    }                           
} 


// writeFile will take the contents from buf and write them to a file.
int writeFile(FILE *file, const void *buf, int len)
{
	const char *pbuf = (const char *)buf;
	while (len > 0)
	{
		int written = fwrite(pbuf, 1, len, file);
		if (written < 1)
		{
			fprintf(stderr, "cannot write to file\n");
			return -1;
		}
		pbuf += written;
		len -= written;
	}
	return 0;
}

// readFile will connect to a socket and save contents to buf.
int readFile(int sockfd, void *buf, int len)
{
	char *pbuf = (char *)buf;
	int total = 0;
	while (len > 0)
	{
		int n = recv(sockfd, pbuf, len, 0);
		if (n < 0)
		{
			fprintf(stderr, "cannot read from socket\n");
			return -1;
		}
		if (n == 0)
		{
			//fprintf(stdout, "socket disconnected\n");
			return 0;
		}
		pbuf += n;
		len -= n;
		total += n;
	}
	return total;
}

// recvFile uses a socket to connect to a server, recive file data and store
// it. It utilizes the functions readFile and writeFile.
void recvFile(int sockfd)
{
	FILE *logFile;
	const char lofFileName[] = "client.log";
	unsigned char checksum[MD5_DIGEST_LENGTH];                                  
	MD5_CTX mdContext;
	int n;
	char buf[BUFFSIZE];

	// Log file.                                                                
    logFile = fopen(lofFileName, "a");                                          
    if (logFile == NULL)                                                        
    {                                                                           
        fprintf(stderr, "Error opening log file\n");                            
        return;                                                                 
    }

	int serverStatus;
	while ((serverStatus = checkSocket(sockfd)) == 0)
	{
		bzero(buf, BUFFSIZE); 
		// File transfered.
		n = readFile(sockfd, buf, FNAMESIZE);
		if (n==0)
			break;
		FILE *file = fopen(buf, "wb");                                       
    	if (file == NULL)                                                           
		{                                                                           
			fprintf(stderr, "Error opening file\n");                                
			return;                                                                 
		}
		fprintf(logFile, "%s ", buf);

		// conventional 32bit call
		// long size = 0;
		int printflag = 0; 
		uint64_t size = 0;
		n = readFile(sockfd, &size, sizeof(size));
		if (n>0)
		{
			// Conventional 32bit call 
			//size = ntohl(size);
			size = be64toh(size);
			if (printflag==0)
			{
				fprintf(logFile, "%ld ", size);
				printflag = 1;
			}

			MD5_Init(&mdContext);
			while (size > 0)
			{
				n = readFile(sockfd, buf, MIN(sizeof(buf), size));
				if (n < 1)
					break;
				MD5_Update(&mdContext, buf, n);
				if (writeFile(file, buf, n) == -1)
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
	}
	fclose(logFile);
}

#endif // __CLIENTOPS_H__
