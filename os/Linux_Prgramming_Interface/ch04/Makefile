CC=gcc
CFLAGS=-Wall -Werror

.PHONY: cp format clean

cp: cp.c error_functions.c
	$(CC) $(CFLAGS) -o $@ $^

format:

clean:
	rm -f cp
