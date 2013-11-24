ifeq ($(OS), Windows_NT)
	CC = gcc
else
	CC = clang
endif

FLAGS = -std=c99 -Wall -O3 -s

tools: font-converter transpose-level

font-converter: font-converter.c
	$(CC) $(FLAGS) font-converter.c -o font-converter

transpose-level: transpose-level.c
	$(CC) $(FLAGS) transpose-level.c -o transpose-level

clean:
	@rm -rf *.o *.exe font-converter transpose-level
