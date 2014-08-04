CC = clang
CFLAGS = -std=c99 -Wall -Wextra
VALGRIND = log_valgrind.txt
ASAN = log_asan.txt
EXE = ZenoEngine

ifeq ($(OS), Windows_NT)
	CC = gcc
	LDFLAGS = -lm -mwindows -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net -lOpenGL32
else
	OS = $(shell uname -s)

	ifeq ($(OS), Linux)
		LDFLAGS = -lm -lSDL2 -lSDL2_image -lSDL2_net -lGL
	else ifeq ($(OS), Darwin)
		LDFLAGS = -lm -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net -framework OpenGL
	endif
endif

OBJECTS = main.o util.o globals.o texture.o level.o player.o font.o animation.o particle.o entity.o

.PHONY: tools clean valgrind asan

zeno: $(OBJECTS)
	@$(CC) $(OBJECTS) $(LDFLAGS) -o $(EXE)

release: clean
	@cp -r data/ bin/
	@cp -r images/ bin/
	@cp -r levels/ bin/
	@cp -r shaders/ bin/
	@mkdir bin/screenshots
	@rm bin/data/*.txt
	@+$(MAKE) CFLAGS="$(CFLAGS) -O3" LDFLAGS="$(LDFLAGS) -s" zeno --no-print-directory

ifeq ($(OS), Windows_NT)
	@cp dll/*.dll bin/
endif

	@mv $(EXE) bin/
	@ln -s bin/$(EXE) .

tools:
	@$(MAKE) -C tools --no-print-directory

clean:
	@rm -rf *.o *.dll $(EXE){,.exe} $(VALGRIND) errors.txt
	@rm -rf bin/*
	@$(MAKE) -C tools clean --no-print-directory

valgrind: clean
	@+$(MAKE) CFLAGS="$(CFLAGS) -g" zeno --no-print-directory
	@echo -e "\nRunning Valgrind..."
	@valgrind --track-origins=yes --leak-check=full ./$(EXE) &> $(VALGRIND)
	@echo "Log saved as $(VALGRIND)."

asan: clean
	@+$(MAKE) CFLAGS="$(CFLAGS) -O3 -g -fsanitize=address -fno-omit-frame-pointer" LDFLAGS="$(LDFLAGS) -g -fsanitize=address" zeno --no-print-directory
	@echo -e "\nRunning AddressSanitizer..."
	@./$(EXE) &> $(ASAN)
	@echo "Log saved as $(ASAN)."

main.o: main.c util.h globals.h texture.h level.h player.h font.h particle.h entity.h
	$(CC) $(CFLAGS) -c main.c

util.o: util.c globals.h level.h
	$(CC) $(CFLAGS) -c util.c

globals.o: globals.c globals.h
	$(CC) $(CFLAGS) -c globals.c

texture.o: texture.c texture.h globals.h
	$(CC) $(CFLAGS) -c texture.c

level.o: level.c level.h globals.h texture.h
	$(CC) $(CFLAGS) -c level.c

player.o: player.c player.h globals.h util.h texture.h level.h animation.h
	$(CC) $(CFLAGS) -c player.c

font.o: font.c font.h globals.h texture.h
	$(CC) $(CFLAGS) -c font.c

animation.o: animation.c animation.h globals.h texture.h
	$(CC) $(CFLAGS) -c animation.c

particle.o: particle.c particle.h globals.h texture.h
	$(CC) $(CFLAGS) -c particle.c

entity.o: entity.c entity.h util.h globals.h texture.h level.h
	$(CC) $(CFLAGS) -c entity.c
