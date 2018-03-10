#include "apue.h"

int main(int argc, char *argv[]){
	printf("Hello system! This is proces # %ld\n", (long)getpid());
	exit(0);
}
