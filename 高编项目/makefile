CC = gcc
CFLAGS = -Wall -O2 -pthread
LDFLAGS = -pthread

TARGET = gateway

# [修改] 加入 process_pool.c
SRCS = main.c cloud_client.c base64.c logger.c password_auth.c config.c daemon.c thread_pool.c udp_server.c io_multiplexing.c token_bucket.c alarm_scheduler.c ipc_modules.c process_pool.c
OBJS = $(SRCS:.c=.o)
# [修改] 加入 process_pool.h
DEPS = gateway.h base64.h logger.h password_auth.h config.h daemon.h thread_pool.h udp_server.h io_multiplexing.h token_bucket.h alarm_scheduler.h ipc_modules.h process_pool.h

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)
	@echo "编译完成！运行 ./$(TARGET) ./iot_gateway.conf 启动网关。"

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
	@echo "清理完成！"

.PHONY: all clean
