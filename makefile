CC = clang
CFLAGS = -std=c99 -Wall -Wextra `sdl2-config --cflags`
LDFLAGS = -lm `sdl2-config --libs` -lSDL2_image -lSDL2_net
VALGRIND = valgrind-log.txt

ifeq ($(OS), Windows_NT)
	CC = gcc
	LDFLAGS += -lOpenGL32
else
	LDFLAGS += -lGL
endif

OBJECTS = main.o util.o globals.o texture.o level.o player.o font.o animation.o particle.o

.PHONY: tools

zeno: $(OBJECTS)
	@$(CC) $(OBJECTS) $(LDFLAGS) -o ZenoEngine

tools:
	@$(MAKE) -C tools --no-print-directory

all: zeno tools

release:
	@$(MAKE) clean --no-print-directory
	@+$(MAKE) CFLAGS="$(CFLAGS) -O3" LDFLAGS="$(LDFLAGS) -s" zeno --no-print-directory

main.o: main.c util.h globals.h texture.h level.h player.h font.h particle.h
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

particle.o: particle.c particle.h globals.h texture.h
	$(CC) $(CFLAGS) -c particle.c

clean:
	@rm -rf *.o ZenoEngine ZenoEngine.exe
	@$(MAKE) -C tools clean --no-print-directory

check:
	@$(MAKE) clean --no-print-directory
	@+$(MAKE) CFLAGS="$(CFLAGS) -g" zeno --no-print-directory
	@echo -e "\nRunning valgrind..."
	@valgrind --track-origins=yes ./ZenoEngine > $(VALGRIND) 2>&1
	@echo "Log saved as $(VALGRIND)."
