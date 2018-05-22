V=
#V=@
CFORMAT=clang-format-6.0

CC=gcc
CFLAGS=-Wall -Werror -Wextra #-DDEBUG

.PHONY: t_sysconf format clean

t_sysconf: t_sysconf.c error_functions.c
	@echo + Building $@ utility...
	$(V)$(CC) $(CFLAGS) -o $@ $^

format:
	@echo + Formatting src...
	$(V)find . -name "*.c" -o -name "*.h" | xargs $(CFORMAT) -i

clean:
	@echo - Cleaning up all executables...
	rm -f t_sysconf
