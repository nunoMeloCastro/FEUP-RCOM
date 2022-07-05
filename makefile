CC=gcc
CFLAGS= -Wall -Werror -pedantic -g

OBJ=protocol.c

all: sender receiver

sender: $(OBJ) sender.c
	$(CC) $(CFLAGS) $^ -o sndr
receiver: $(OBJ) receiver.c
	$(CC) $(CFLAGS) $^ -o recv

.PHONY: clean setup
setup:
	sudo socat -d -d PTY,link=/dev/ttyS10,mode=777 PTY,link=/dev/ttyS11,mode=777
clean:
	rm -f sndr recv
