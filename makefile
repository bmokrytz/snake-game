CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -mwindows -municode
OUTPUT = snake.exe
SRC = snake.c

all: $(OUTPUT)

$(OUTPUT): $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC) -o $(OUTPUT)

clean:
	rm $(OUTPUT)
