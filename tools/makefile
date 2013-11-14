CC = clang
FLAGS = -std=c99 -Wall -s -O3

tools: font-converter transpose-level

font-converter: font-converter.c
	$(CC) $(FLAGS) font-converter.c -o font-converter

transpose-level: transpose-level.c
	$(CC) $(FLAGS) transpose-level.c -o transpose-level

clean:
	@rm -rf *.o font-converter transpose-level
