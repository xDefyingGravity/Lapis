CC = clang
CFLAGS = -Wall -Wextra -std=c99 -Wunused-variable -I.
LDFLAGS = -lpng
SRC = src/lapis.c
OBJ = $(SRC:.c=.o)

all: lapis

lapis: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
clean:
	rm -f lapis $(OBJ)
	
.PHONY: clean all lapis
