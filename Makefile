CC=clang
CFLAGS=-g -Werror -Wall -Iinclude

C_FILES := $(wildcard src/*.c)
O_FILES := $(addprefix obj/,$(notdir $(C_FILES:.c=.o)))

all: $(O_FILES)

test:

obj/%.o: src/%.c
	 $(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf obj/*.o 
