CC = clang
CFLAGS = -std=c99 -Wall -Wextra

EXE = ZenoEngine

LOG = log.txt
ASAN = log_asan.txt
VALGRIND = log_valgrind.txt

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

# TODO: Put .o files in a separate directory.
OBJECTS = animation.o entity.o font.o globals.o level.o main.o particle.o player.o texture.o util.o

.PHONY: asan clean tools valgrind

# TODO: Copy files to bin/ and make a symbolic link to the executable.
zeno: $(OBJECTS)
	@$(CC) $(OBJECTS) $(LDFLAGS) -o $(EXE)

asan: clean
	@+$(MAKE) CFLAGS="$(CFLAGS) -O3 -g -fsanitize=address -fno-omit-frame-pointer" LDFLAGS="$(LDFLAGS) -g -fsanitize=address" zeno --no-print-directory

	@echo -e "\nRunning AddressSanitizer..."
	@./$(EXE) &> $(ASAN)
	@echo "Log saved as $(ASAN)."

clean:
	@rm -rf *.o *.dll $(EXE){,.exe} $(LOG) $(ASAN) $(VALGRIND)
	@rm -rf bin/
	@$(MAKE) -C tools clean --no-print-directory

# TODO: Automatically rebuild font files.
release: clean
	@+$(MAKE) CFLAGS="$(CFLAGS) -O3" LDFLAGS="$(LDFLAGS) -s" zeno --no-print-directory

	@mkdir -p bin/screenshots
	@cp -r data/ bin/
	@rm bin/data/*.txt
	@cp -r images/ bin/
	@cp -r levels/ bin/
	@cp -r shaders/ bin/

ifeq ($(OS), Windows_NT)
	@cp dll/*.dll bin/
endif

	@mv $(EXE) bin/
	@ln -s bin/$(EXE) .

tools:
	@$(MAKE) -C tools --no-print-directory

valgrind: clean
	@+$(MAKE) CFLAGS="$(CFLAGS) -g" zeno --no-print-directory

	@echo -e "\nRunning Valgrind..."
	@valgrind --track-origins=yes --leak-check=full ./$(EXE) &> $(VALGRIND)
	@echo "Log saved as $(VALGRIND)."

animation.o: animation.c animation.h globals.h texture.h
	$(CC) $(CFLAGS) -c animation.c

entity.o: entity.c entity.h util.h globals.h texture.h level.h
	$(CC) $(CFLAGS) -c entity.c

font.o: font.c font.h globals.h texture.h
	$(CC) $(CFLAGS) -c font.c

globals.o: globals.c globals.h
	$(CC) $(CFLAGS) -c globals.c

level.o: level.c level.h globals.h texture.h
	$(CC) $(CFLAGS) -c level.c

main.o: main.c util.h globals.h texture.h level.h player.h font.h particle.h entity.h
	$(CC) $(CFLAGS) -c main.c

particle.o: particle.c particle.h globals.h texture.h
	$(CC) $(CFLAGS) -c particle.c

player.o: player.c player.h globals.h util.h texture.h level.h animation.h
	$(CC) $(CFLAGS) -c player.c

texture.o: texture.c texture.h globals.h
	$(CC) $(CFLAGS) -c texture.c

util.o: util.c globals.h level.h
	$(CC) $(CFLAGS) -c util.c
