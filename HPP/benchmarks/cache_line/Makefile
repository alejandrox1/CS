CC=gcc
CFLAGS=-g -Wall -Wextra #-DDEBUG

.PHONY: line_size clean

line_size: cache-line.c
	@echo + Determine cache line size...
	$(CC) $(CFLAGS) -o $@ $^

clean:
	@echo - Removing all executables...
	rm -f line_size
