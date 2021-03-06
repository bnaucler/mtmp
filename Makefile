CC = cc
TARGET = mtmp
SOURCE = *.c
DESTDIR = /usr/bin
CFLAGS ?= -Wall -g -ljansson -lcurl -lGeoIP

all:
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

clean:
	rm -f $(TARGET)

install:
	cp $(TARGET) $(DESTDIR)
