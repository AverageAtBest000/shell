CC = clang
FLAGS = -Wextra -Wall

shell: main builtins executor input parser
	$(CC) main builtins executor input parser -o shell

main: main.c
	$(CC) main.c -c main

builtins: builtins.c
	$(CC) builtins.c -c builtins

executor: executor.c
	$(CC) executor.c -c executor

input: input.c
	$(CC) input.c -c input

parser: parser.c
	$(CC) parser.c -c parser
