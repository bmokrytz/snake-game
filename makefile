CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude
LDFLAGS = -mwindows -municode
LIBS = -lmsimg32

OUTPUT = snake.exe

SRC = src/main.c src/game.c src/snakeWin32.c src/log.c
OBJ = $(SRC:.c=.o)

all: $(OUTPUT)

$(OUTPUT): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS) $(LIBS)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(OUTPUT)
