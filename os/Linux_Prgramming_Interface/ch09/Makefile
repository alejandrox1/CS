V=
#V=@

CC=gcc
CFLAGS=-Wall -Werror -Wextra #-DDEBUG

.PHONY: idshow format clean

idhsow: idshow.c error_functions.c ugid_functions.c
	@echo + Building $@ utility...
	$(V)$(CC) $(CFLAGS) -o $@ $^

format:
	@echo + Formatting src...
	$(V)find . -name "*.c" -o -name "*.h" | xargs clang-format -i

clean:
	@echo - Cleaning up all executables...
	rm -f check_passwd
