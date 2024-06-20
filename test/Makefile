CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -llapis -lpng -L../ -I../ -Ilapis

test: test.c ../liblapis.a
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f test
