EXECS=main
MPICC?=mpicc

all: ${EXECS}

main: main.c
	${MPICC} -o hello_world main.c

clean:
	rm hello_world
