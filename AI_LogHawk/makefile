CC = gcc
CFLAGS = -Wall -g
SRC = loghawk.c collector.c processor.c outputer.c
OBJ = $(SRC:.c=.o)
TARGET = loghawk

$(TARGET): $(OBJ)
	$(CC) -o $@ $^

clean:
	rm -f $(TARGET) $(OBJ) output_result.log
