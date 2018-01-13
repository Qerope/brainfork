CC = gcc
CFLAGS = -O2 -Wall -Wextra
.PHONY: test
test: bf
	sh tests/run.sh
bf: bf.c
	$(CC) $(CFLAGS) -o $@ bf.c

clean:
	rm -f bf
