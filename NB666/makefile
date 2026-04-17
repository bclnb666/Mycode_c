CC=gcc
CFLAGS=-Wall -Wextra -O3 -std=c99 -D_POSIX_C_SOURCE=200809L
LDFLAGS=-lpthread -lcrypt

SOURCES=main.c daemon.c io_multiplexing.c thread_pool.c ipc_modules.c token_bucket.c alarm_scheduler.c password_auth.c log.c
OBJECTS=$(SOURCES:.c=.o)
TARGET=iot_gateway

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o $(TARGET)
