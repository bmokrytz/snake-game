CC      = gcc
CFLAGS  = -Wall -Wextra -g -Iinclude
LDFLAGS = -mwindows -municode
LIBS    = -lmsimg32 -lgdi32 -luser32

OUTPUT = snake.exe

SRC = \
    src/main.c \
    src/game.c \
    src/platform.c \
    src/snakeWin32.c \
    src/log.c

OBJ = $(SRC:.c=.o)

# Default target (this MUST be first)
all: $(OUTPUT)

# Link step (this is what you were missing)
$(OUTPUT): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS) $(LIBS)

# Compile step (puts .o files beside their .c files in src/)
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(OUTPUT)
