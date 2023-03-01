CC     = gcc
CFLAGS = -Wall -Wextra #-DDEBUG
#CFLAGS = -Wall -Wextra -DHAVE_SSL #-DDEBUG
CLIBS  = -lbluetooth
PROG   = blueshell
OBJ    = blueshell.o bluetooth.o client.o logger.o util.o proto.o server.o

$(PROG): main.c $(OBJ)

	$(CC) $(CFLAGS) -o $(PROG)\
         main.c $(OBJ) $(CLIBS)


blueshell.o: blueshell.c
	$(CC) $(CFLAGS) -c blueshell.c
bluetooth.o: bluetooth.c
	$(CC) $(CFLAGS) -c bluetooth.c
logger.o: logger.c
	$(CC) $(CFLAGS) -c logger.c
util.o: util.c
	$(CC) $(CFLAGS) -c util.c
client.o: client.c
	$(CC) $(CFLAGS) -c client.c
proto.o: proto.c
	$(CC) $(CFLAGS) -c proto.c
server.o: server.c
	$(CC) $(CFLAGS) -c server.c

clean:
	 rm -rf *.o $(PROG)

