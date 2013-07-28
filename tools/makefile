CC = clang
FLAGS = -std=c99 -Wall -s

tools: font-converter

font-converter: font-converter.c
	$(CC) $(FLAGS) font-converter.c -o font-converter

clean:
	@rm -rf *.o font-converter
