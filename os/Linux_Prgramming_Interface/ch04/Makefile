V=
#V=@

CC=gcc
CFLAGS=-Wall -Werror

.PHONY: cp seek format clean

cp: cp.c error_functions.c
	@echo + Building cp utility...
	$(V)$(CC) $(CFLAGS) -o $@ $^

seek: seek_io.c error_functions.c get_num.c
	@echo + Building seek_io...
	$(V)$(CC) $(CFLAGS) -o $@ $^

format:
	@echo + Formatting src...
	$(V)find . -name "*.c" -o -name "*.h" | xargs clang-format -i

clean:
	@echo - Cleaning up all executables...
	rm -f cp
	rm -f seek
