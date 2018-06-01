SRC=mpi_hello_world.c
EXEC=${SRC:.c=}
MPICC?=mpicc

LAUNCHER=mpirun
n?=12

.PHONY: all clean

all: ${EXEC}
	${LAUNCHER} -np ${n} ./${EXEC}

${EXEC}: ${SRC}
	${MPICC} -o ${EXEC} $^

clean:
	rm -f ${EXEC}
