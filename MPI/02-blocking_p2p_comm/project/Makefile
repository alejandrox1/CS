MPICC?=mpicc

.PHONY: send_recv ping_pong ring clean

send_recv: send_recv.c
	${MPICC} -o $@ $<

ping_pong: ping_pong.c
	${MPICC} -o $@ $<

ring: ring.c
	${MPICC} -o $@ $<

clean:
	rm -f send_recv ping_pong ring
