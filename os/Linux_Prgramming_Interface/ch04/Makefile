V=
#V=@

CC=gcc
CFLAGS=-Wall -Werror

.PHONY: cp format clean

cp: cp.c error_functions.c
	@echo + Building cp utility...
	$(V)$(CC) $(CFLAGS) -o $@ $^

format:
	@echo + Formatting src...
	$(V)find . -name "*.c" -o -name "*.h" | xargs clang-format -i

clean:
	rm -f cp
