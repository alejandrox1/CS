MPICC?=mpicc

EXECS=random_walk


all: $(EXECS)

%: %.c
	$(MPICC) -o $@ $<


.PHONY: all clean

clean:
	rm -f $(EXECS)
