CC = clang
CFLAGS = -std=c99 -Wall
LDFLAGS = -lSDL2 -lSDL2_image -lGL -lm -s

OBJECTS = main.o globals.o texture.o level.o player.o font.o lodepng.o

.PHONY: tools

all: zeno tools

zeno: $(OBJECTS)
	@$(CC) $(OBJECTS) $(LDFLAGS) -o ZenoEngine

main.o: main.c globals.h texture.h level.h player.h font.h
	$(CC) $(CFLAGS) -c main.c

globals.o: globals.c globals.h
	$(CC) $(CFLAGS) -c globals.c

texture.o: texture.c texture.h globals.h lodepng.h
	$(CC) $(CFLAGS) -c texture.c

level.o: level.c level.h globals.h texture.h
	$(CC) $(CFLAGS) -c level.c

player.o: player.c player.h globals.h texture.h level.h
	$(CC) $(CFLAGS) -c player.c

font.o: font.c font.h globals.h texture.h
	$(CC) $(CFLAGS) -c font.c

lodepng.o: lodepng.c lodepng.h
	$(CC) $(CFLAGS) -DLODEPNG_NO_COMPILE_ANCILLARY_CHUNKS -DLODEPNG_NO_COMPILE_ERROR_TEXT -c lodepng.c

tools:
	@$(MAKE) -C tools --no-print-directory

clean:
	@rm -rf *.o ZenoEngine
	@$(MAKE) -C tools clean --no-print-directory
