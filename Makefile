CC = clang
FLAGS = -Wextra -Wall

shell: main builtins executor input parser
	$(CC) main builtins executor input parser -o shell

main: main.c  
	$(CC) main.c -c main

builtins: builtins.c builtins.h
	$(CC) builtins.c -c builtins

executor: executor.c executor.h
	$(CC) executor.c -c executor

input: input.c input.h
	$(CC) input.c -c input

parser: parser.c parser.h
	$(CC) parser.c -c parser
