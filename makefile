CC = clang
CFLAGS = -std=c99 -Wall
LDFLAGS = -lSDL2 -lSDL2_image -lGL -lm -s

OBJECTS = main.o util.o globals.o texture.o level.o player.o font.o animation.o

.PHONY: tools

all: zeno tools

zeno: $(OBJECTS)
	@$(CC) $(OBJECTS) $(LDFLAGS) -o ZenoEngine

main.o: main.c util.h globals.h texture.h level.h player.h font.h
	$(CC) $(CFLAGS) -c main.c

util.o: util.c globals.h
	$(CC) $(CFLAGS) -c util.c

globals.o: globals.c globals.h
	$(CC) $(CFLAGS) -c globals.c

texture.o: texture.c texture.h globals.h
	$(CC) $(CFLAGS) -c texture.c

level.o: level.c level.h globals.h texture.h
	$(CC) $(CFLAGS) -c level.c

player.o: player.c player.h globals.h texture.h level.h animation.h
	$(CC) $(CFLAGS) -c player.c

font.o: font.c font.h globals.h texture.h
	$(CC) $(CFLAGS) -c font.c

animation.o: animation.c animation.h globals.h texture.h
	$(CC) $(CFLAGS) -c animation.c

tools:
	@$(MAKE) -C tools --no-print-directory

clean:
	@rm -rf *.o ZenoEngine
	@$(MAKE) -C tools clean --no-print-directory
