CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -mwindows -municode
OUTPUT = snake.exe
SRC = snake.c

all: $(OUTPUT)

$(OUTPUT): $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC) -o $(OUTPUT) -lmsimg32

clean:
	rm $(OUTPUT)
