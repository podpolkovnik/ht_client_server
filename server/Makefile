CC = gcc
PROG ?= htserver
OBJS = array.o hashtable.o hcsprotocol.o tcpserver.o value.o main.o
INC = -I./include
SRC = ./src
CFLAGS = -c -Wall

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(INC) $(OBJS) -o $(PROG)

array.o: $(SRC)/array.c
	$(CC) $(INC) $(CFLAGS) $(SRC)/array.c

hashtable.o: $(SRC)/hashtable.c
	$(CC) $(INC) $(CFLAGS) $(SRC)/hashtable.c

hcsprotocol.o: $(SRC)/hcsprotocol.c
	$(CC) $(INC) $(CFLAGS) $(SRC)/hcsprotocol.c

tcpserver.o: $(SRC)/tcpserver.c
	$(CC) $(INC) $(CFLAGS) $(SRC)/tcpserver.c

value.o: $(SRC)/value.c
	$(CC) $(INC) $(CFLAGS) $(SRC)/value.c

main.o: $(SRC)/main.c
	$(CC) $(INC) $(CFLAGS) $(SRC)/main.c

clean:
	rm -rf *.o $(PROG)