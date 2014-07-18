ifeq ($(OS), Windows_NT)
	CC = gcc
else
	CC = clang
endif

FLAGS = -std=c99 -Wall -Wextra -pedantic -O3 -s

# Should targets be PHONY?
tools: font-converter transpose-level

font-converter: font-converter.c
	$(CC) $(FLAGS) font-converter.c -o font-converter

transpose-level: transpose-level.c
	$(CC) $(FLAGS) transpose-level.c -o transpose-level

clean:
	@rm -rf *.o font-converter{,.exe} transpose-level{,.exe}
