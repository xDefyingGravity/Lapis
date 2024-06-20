CC = clang
CFLAGS = -Wall -Wextra -std=c99 -Wunused-variable -I.
LDFLAGS = -lpng
RELEASE_DIRS = lapis_release lapis_release/lib lapis_release/include lapis_release/bin lapis_release/src
SRC = lapis.c
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

release: $(RELEASE_DIRS)
	cp liblapis.a lapis_release/lib/
	cp lapis.h lapis_release/include/
	cp lapis.c lapis_release/src/
	@C_CONTENT="$$(cat lapis.c)"; \
	echo "#define PROD" > lapis_release/src/lapis.c; \
	echo "$$C_CONTENT" >> lapis_release/src/lapis.c
	cp util/release.make lapis_release/Makefile

$(RELEASE_DIRS):
	mkdir -p $@

.PHONY: clean all lapis lib release
