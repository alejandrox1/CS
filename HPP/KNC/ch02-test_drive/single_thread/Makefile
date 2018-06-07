CC?=gcc
CFLAGS?=-Wall -Wextra

.PHONY: helloflops1 clean

helloflops1: helloflops1.c
	@echo -e "$${green} + Building $@ ...$${reset}"
	$(CC) $(CFLAGS) -o $@ $^

clean:
	@echo -e "$${green} + Removing executables...$${reset}"
	rm -f helloflops1
