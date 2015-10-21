CC=clang
CFLAGS=-Werror -Wall -Iinclude
DEBUGFLAGS=-g -O0

all: test_sponge

test_sponge:
	$(CC) $(CFLAGS) $(DEBUGFLAGS) src/sponge.c tests/test_sponge.c -obin/test_sponge

clean:
	rm -rf obj/*.o bin/*
