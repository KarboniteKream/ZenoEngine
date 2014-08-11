NAME = ZenoEngine
ARCH ?= 64

CC = clang
CFLAGS = -std=c99 -Wall -Wextra

SRCDIR = src
OBJDIR = obj

LOG = log.txt
ASAN = log_asan.txt
VALGRIND = log_valgrind.txt

ifeq ($(OS), Windows_NT)
	CC = gcc
	LDFLAGS = -lm -mwindows -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net -lOpenGL32
else ifeq ($(shell uname), Linux)
	LDFLAGS = -lm -lSDL2 -lSDL2_image -lSDL2_net -lGL
else
	LDFLAGS = -lm -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net -framework OpenGL
endif

OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(wildcard $(SRCDIR)/*.c))

.PHONY: asan clean cppcheck tools valgrind

zeno: $(OBJECTS)
	@mkdir -p screenshots
	@$(CC) $(OBJECTS) $(LDFLAGS) -o $(NAME)

# TODO: Automatically rebuild font files.
release: clean
	@mkdir -p bin/screenshots
	@cp -r data/ images/ levels/ shaders/ bin/
	@rm bin/data/*.txt

ifeq ($(OS), Windows_NT)
	@cp extra/dll/win$(ARCH)/* bin/
endif

	@$(MAKE) CFLAGS="$(CFLAGS) -O3" LDFLAGS="$(LDFLAGS) -s" zeno --no-print-directory
	@mv $(NAME) bin/
	@ln -s bin/$(NAME) .

asan: clean
	@+$(MAKE) CFLAGS="$(CFLAGS) -O3 -g -fsanitize=address -fno-omit-frame-pointer" LDFLAGS="$(LDFLAGS) -g -fsanitize=address" zeno --no-print-directory

	@echo -e "\nRunning AddressSanitizer..."
	@./$(NAME) &> $(ASAN)
	@echo "Log saved as $(ASAN)."

clean:
	@rm -rf bin/ obj/ $(NAME){,.exe} $(LOG) $(ASAN) $(VALGRIND)
	@$(MAKE) -C tools clean --no-print-directory

cppcheck:
	@cppcheck . --enable=all --suppress=missingIncludeSystem --inconclusive --std=c99 --verbose

tools:
	@$(MAKE) -C tools --no-print-directory

valgrind: clean
	@+$(MAKE) CFLAGS="$(CFLAGS) -g" LDFLAGS="$(LDFLAGS) -g" zeno --no-print-directory

	@echo -e "\nRunning Valgrind..."
	@valgrind --track-origins=yes --leak-check=full ./$(NAME) &> $(VALGRIND)
	@echo "Log saved as $(VALGRIND)."

$(OBJDIR)/animation.o: $(addprefix $(SRCDIR)/, animation.c animation.h globals.h texture.h)
$(OBJDIR)/entity.o: $(addprefix $(SRCDIR)/, entity.c entity.h globals.h level.h texture.h util.h)
$(OBJDIR)/font.o: $(addprefix $(SRCDIR)/, font.c font.h globals.h texture.h)
$(OBJDIR)/globals.o: $(addprefix $(SRCDIR)/, globals.c globals.h)
$(OBJDIR)/level.o: $(addprefix $(SRCDIR)/, level.c globals.h level.h texture.h)
$(OBJDIR)/main.o: $(addprefix $(SRCDIR)/, main.c entity.h font.h globals.h level.h particle.h player.h texture.h util.h)
$(OBJDIR)/particle.o: $(addprefix $(SRCDIR)/, particle.c globals.h particle.h texture.h)
$(OBJDIR)/player.o: $(addprefix $(SRCDIR)/, player.c animation.h globals.h level.h player.h texture.h util.h)
$(OBJDIR)/texture.o: $(addprefix $(SRCDIR)/, texture.c globals.h texture.h)
$(OBJDIR)/util.o: $(addprefix $(SRCDIR)/, util.c globals.h level.h util.h)

$(OBJECTS): | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	@mkdir $(OBJDIR)
