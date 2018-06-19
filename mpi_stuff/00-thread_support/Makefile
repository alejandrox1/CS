MPICC?=mpicc

EXECS=thread_support


all: $(EXECS)

%: %.c
	$(MPICC) -o $@ $<


.PHONY: all clean

clean:
	rm -f $(EXECS)
