CC = gcc
CFLAGS = -Wall -Wextra -pthread
OBJS = main.o server.o utils.o

all: echo_server

echo_server: $(OBJS)
	$(CC) $(CFLAGS) -o echo_server $(OBJS)

main.o: main.c server.h
	$(CC) $(CFLAGS) -c main.c

server.o: server.c server.h utils.h
	$(CC) $(CFLAGS) -c server.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

clean:
	rm -f echo_server $(OBJS)