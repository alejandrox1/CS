MPICC?=mpicc

EXECS=check_status


all: $(EXECS)

%: %.c
	$(MPICC) -o $@ $<

.PHONY: all clean

clean:
	rm -f $(EXECS)
