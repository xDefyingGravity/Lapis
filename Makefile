CC = clang
CFLAGS = -Wall -Wextra -std=c99 -Wunused-variable -I.
LDFLAGS = -lpng
SRC = lapis.c main.c
OBJ = $(SRC:.c=.o)

all: lapis

lapis: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

lib: $(OBJ)
	ar rcs liblapis.a $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f lapis liblapis.a $(OBJ)

test:
	$(CC) $(CFLAGS) -o lapis_test test/test.c lapis.c -I. -lpng

.PHONY: clean all lapis lib test
