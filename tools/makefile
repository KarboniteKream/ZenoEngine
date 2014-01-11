ifeq ($(OS), Windows_NT)
	CC = gcc
else
	CC = clang
endif

FLAGS = -std=c99 -Wall -O3 -s

tools: font-converter transpose-level network-client

font-converter: font-converter.c
	$(CC) $(FLAGS) font-converter.c -o font-converter

transpose-level: transpose-level.c
	$(CC) $(FLAGS) transpose-level.c -o transpose-level

network-client: network-client.c
	$(CC) $(FLAGS) `sdl2-config --cflags` network-client.c -o network-client `sdl2-config --libs` -lSDL2_net

clean:
	@rm -rf *.o *.exe font-converter transpose-level network-client
