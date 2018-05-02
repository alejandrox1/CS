CC=gcc
CFLAGS=-Wall -Werror

.PHONY: cp format clean

cp: cp.c error_functions.c
	$(CC) $(CFLAGS) -o $@ $^

format:
	find . -name "*.c" -o -name "*.h" | xargs clang-format -i

clean:
	rm -f cp
