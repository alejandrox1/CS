#include <stdio.h>
#include <stdlib.h>

int main()
{
	char buff[1024*1024];
	printf("(1024*1024) = %d\tsize = %ld\n", (1024*1024), sizeof(buff));
	exit(0);
}

