V=
#V=@

CC=gcc
CFLAGS=-Wall -Werror -Wextra

.PHONY: check_passwd format clean

check_passwd: check_passwd.c get_pass.c error_functions.c
	@echo + Building check_passwd utility...
	$(V)$(CC) $(CFLAGS) -o $@ $^

format:
	@echo + Formatting src...
	$(V)find . -name "*.c" -o -name "*.h" | xargs clang-format -i

clean:
	@echo - Cleaning up all executables...
	rm -f check_passwd
