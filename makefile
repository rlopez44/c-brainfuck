# Used for compiling the Brainfuck interpreter
CC = gcc
CFLAGS = -g

# Targets
all: directories brainfuck

# Compile the binary with debug symbols
debug: directories brainfuck-debug

directories:
	@mkdir -p ./bin/
	@mkdir -p ./obj/

brainfuck: obj/main.o obj/utils.o obj/tokenize.o obj/interpreter.o
	$(CC) -o bin/$@ $^

brainfuck-debug: obj/main.o obj/utils.o obj/tokenize.o obj/interpreter.o
	$(CC) $(CFLAGS) -o bin/brainfuck $^

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean the object files directory
clean:
	@rm -rf ./obj/

# Clean the object files and binary directories
full-clean: clean
	@rm -rf ./bin/
