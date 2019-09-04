CC=gcc 
LDFLAGS= -lpcap  -lbson-1.0
CFLAGS= -Wall -g -I.
SOURCE= mongo-sniffer.c mongo.c
OBJS=$(SOURCE:.cc=.o)
TARGET= mongo-sniffer

.c.o:
	$(CC) $(CFLAGS) $< -o $@

all: release

x: 
	./mongo-sniffer -i lo
d:
	jdebug=true ./mongo-sniffer -i lo

release: $(OBJS)
	$(CC) $(CFLAGS)  -o $(TARGET) $^ $(LDFLAGS)

clean:
	rm -f  *.o  $(TARGET)

