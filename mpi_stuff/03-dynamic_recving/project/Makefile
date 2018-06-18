MPICC?=mpicc

EXECS=check_status probe


all: $(EXECS)

%: %.c
	$(MPICC) -o $@ $<

.PHONY: all clean

clean:
	rm -f $(EXECS)
