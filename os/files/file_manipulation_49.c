#include <unistd.h>
#include <stdio.h>
#include <fcntl.h> 
int main() {
	int inFile, outFile;
	char *inFileName = "in_test";
	char *outFileName = "out_test";
	int len;
	char c;

	inFile = open(inFileName, O_RDONLY);
	outFile = open(outFileName, O_WRONLY);
	/* Loop through the input file */
	while ((len = read(inFile, &c, 1)) > 0) 
	write(outFile, &c, 1);
	
	/* CLose files and quit */;
	close(inFile);
	close(outFile);
}
