MPICC?=mpic++

EXECS=random_walk


all: $(EXECS)

%: %.cc
	$(MPICC) -o $@ $<


.PHONY: all clean

clean:
	rm -f $(EXECS)
