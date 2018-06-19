CC = g++
CFLAGS = -Wall -Wextra -Wshadow -Wfloat-equal
SRCS = $(wildcard src/*.cpp)
NAME = 1612180_1612628_1612403.exe

all:
	$(CC) $(CFLAGS) $(SRCS) -o $(NAME)

debug:
	$(CC) $(CFLAGS) $(SRCS) -g -o $(NAME)

mem: all
	drmemory.exe $(NAME)

int: all
	$(NAME) int input.txt output.txt

float: all
	$(NAME) float input.txt output.txt

clean:
	del *.exe
