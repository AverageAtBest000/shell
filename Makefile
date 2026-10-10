CC = clang
LIBDS_INCLUDE = build/_deps/libds-c-src/include
FLAGS = -std=gnu23 -Wextra -Wall -Iinclude -I$(LIBDS_INCLUDE)

shell: main.o builtins.o executor.o input.o parser.o completion.o handlers.o
	$(CC) main.o builtins.o executor.o input.o parser.o completion.o handlers.o -o shell

main.o: src/main.c include/builtins.h include/executor.h include/input/input.h include/parser.h
	$(CC) $(FLAGS) -c src/main.c

builtins.o: src/builtins.c include/builtins.h
	$(CC) $(FLAGS) -c src/builtins.c

executor.o: src/executor.c include/executor.h
	$(CC) $(FLAGS) -c src/executor.c

input.o: src/input/input.c include/input/input.h include/input/handlers.h
	$(CC) $(FLAGS) -c src/input/input.c

parser.o: src/parser.c include/parser.h
	$(CC) $(FLAGS) -c src/parser.c

completion.o: src/input/completion.c include/input/completion.h include/parser.h
	$(CC) $(FLAGS) -c src/input/completion.c

handlers.o: src/input/handlers.c include/input/handlers.h include/input/input.h include/input/completion.h
	$(CC) $(FLAGS) -c src/input/handlers.c

main.o input.o parser.o completion.o handlers.o: $(LIBDS_INCLUDE)/ds_string.h

$(LIBDS_INCLUDE)/ds_string.h:
	cmake -S . -B build
