MPICC?=mpicc
MPICXX?=mpic++

EXECS=random_walk


all: $(EXECS)

%: %.c
	$(MPICC) -o $@ $<

%: %.cc
	$(MPICXX) -o $@ $<


.PHONY: all clean

clean:
	rm -f $(EXECS)
