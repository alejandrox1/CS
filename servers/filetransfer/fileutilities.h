#ifndef __FILEUTILS_H__
#define __FILEUTILS_H__

#include <stdio.h>                                                              
#include <stdlib.h>                                                             
#include <unistd.h>                                                             
#include <ctype.h>
#include <dirent.h>
#include <string.h>
#include "errutilities.h"

#define NFILES 1000
#define FNAMESIZE 10 


// parseExtOUTFiles is a helper func for scandir, used to get all files with 
// an ".out" extension.
static int parseExtOUTFiles(const struct dirent *dir)
{
	if (!dir)
		return 0;
	if (dir->d_type == DT_REG)
	{
		char *ext = strrchr(dir->d_name, '.');
		if ((!ext) || (ext == dir->d_name))
			return 0;
		else
			if (strcmp(ext, ".out") == 0)
				return 1;
	}
	return 0;
}


void listFiles(char filenames[NFILES][FNAMESIZE])
{
	char errmsg[ERRMSG];
	struct dirent **namelist;
	int n = scandir(".", &namelist, parseExtOUTFiles, alphasort);
	if (n < 0)
	{
		getError(n, errmsg);                                               
	    fprintf(stderr, "scandir error: %s\n", errmsg);
		return;
	}
	else
	{
		while (n--) 
		{
			strcpy(filenames[n], namelist[n]->d_name);
			//printf("%s\n", namelist[n]->d_name);
			free(namelist[n]);
		}
		free(namelist);
	}
}


// generateRandomStr will generate a seqLen filename into name for a given file
// extension type.
void generateRandomStr(char name[], char *ext, int seqLen)
{
	//srand(time(NULL));

	int n, i=0;
	while (i < seqLen)
	{
		// ASCII is between 0 an 127.
		n = rand()%128;
		if (isalpha(n) || isdigit(n))
		{
			name[i] = (char)n;
			i++;
		}
	}
	strcat(name, ext);
}


// createDevUrandom will use /dev/urandom as input to create a file of size 
// ranging from 1MB to 10MB.
void createDevUrandomFile(const char *filename)                                                                      
{                                                                               
    //srand(time(NULL));    
    int factor = rand()%10 + 1;
                                                
    char buf[1024*1024];                                                        
    FILE *input = fopen("/dev/urandom", "r"), 
		 *output = fopen(filename, "w");         

	if (input == NULL)
	{
		fprintf(stderr, "cannot open /dev/urandom\n");
		return;
	}
	if (output == NULL)
	{
		fprintf(stderr, "createDevUrandom cannot open filename\n");
		return;
	}

	int i, n;
    for (i=0; i<factor; i++)                                                      
    {                                                                           
        n = fread(buf, 1, sizeof(buf), input);                                  
        if (n < 1024)    
		{
            fprintf(stderr, "error reading from /dev/urandom\n");
			return;
		}                                                                       
        n = fwrite(buf, 1, sizeof(buf), output);                                
        if (n < 1024)   
		{
            fprintf(stderr, "error in createDevUrandom while writing to file\n");   
			return;
		}

    }                                                                           
    fclose(input);                                                              
    fclose(output);                                                             
}  

#endif // __FILEUTILS_H__
