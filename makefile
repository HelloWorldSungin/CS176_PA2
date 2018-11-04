CC = gcc
ARGS = -Wall

all: server_c_udp client_c_udp

server_c_udp: server_c_udp.c
	$(CC) $(ARGS) -o server_c_udp server_c_udp.c

client_c_udp: client_c_udp.c
	$(CC) $(ARGS) -o client_c_udp client_c_udp.c

clean:
	rm -f *.o server_c_udp *~
	rm -f *.o client_c_udp *~
