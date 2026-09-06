CC = clang
FLAGS = -Wextra -Wall -Iinclude

shell: main.o builtins.o executor.o input.o parser.o
	$(CC) main.o builtins.o executor.o input.o parser.o -o shell

main.o: src/main.c include/builtins.h include/executor.h include/input.h include/parser.h
	$(CC) $(FLAGS) -c src/main.c

builtins.o: src/builtins.c include/builtins.h
	$(CC) $(FLAGS) -c src/builtins.c

executor.o: src/executor.c include/executor.h
	$(CC) $(FLAGS) -c src/executor.c

input.o: src/input.c include/input.h
	$(CC) $(FLAGS) -c src/input.c

parser.o: src/parser.c include/parser.h
	$(CC) $(FLAGS) -c src/parser.c
