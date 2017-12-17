CC = gcc
CFLAGS = -O2 -Wall -Wextra
bf: bf.c
	$(CC) $(CFLAGS) -o $@ bf.c

clean:
	rm -f bf
