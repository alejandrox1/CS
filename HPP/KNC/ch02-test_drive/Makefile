CC?=gcc
CFLAGS?=-Wall -Wextra

.PHONY: helloflops1 clean

helloflops1: helloflops1.c
	@echo "$${green} + Building $@ ...$${reset}"
	$(CC) $(CFLAGS) -o $@ $^

clean:
	@echo "$${green} + Removing executables...$${reset}"
	rm -f helloflops1
