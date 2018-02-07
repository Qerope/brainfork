CC = gcc
CFLAGS = -O2 -Wall -Wextra
CFLAGS += -DUSE_COMPUTED_GOTO
.PHONY: test
test: bf
	sh tests/run.sh
bf: bf.c
	$(CC) $(CFLAGS) -o $@ bf.c

clean:
	rm -f bf
