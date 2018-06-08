MPICC?=mpicc

.PHONY: send_recv clean

send_recv: send_recv.c
	${MPICC} -o $@ $<

clean:
	rm -f send_recv
