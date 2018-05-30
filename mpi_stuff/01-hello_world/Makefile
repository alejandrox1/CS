EXECS=hello_world
MPICC?=mpicc

.PHONY: all clean

${EXECS}: main.c
	${MPICC} -o ${EXECS} main.c

clean:
	rm -f ${EXECS}
