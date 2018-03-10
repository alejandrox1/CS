#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>


int main()
{
	char name[10];

	srand(time(NULL));
	int i=0, n;
	while (i<5)
	{
		n = rand()%128;
		if (isalpha(n) || isdigit(n))
		{
			name[i] = (char)n;
			i++;
		}

	}

	strcat(name, ".out");
	printf("filename: '%s', size: %ld\n", name, sizeof(name));

	return 0;
}
