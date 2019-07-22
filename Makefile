CC = gcc
CFLAGS  = -g -Wall -std=c99 -fPIC
prefix = /usr
TARGET = overscan

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	-rm -f $(TARGET)

distclean: clean

install: $(TARGET)
	install -D $(TARGET) $(DESTDIR)$(prefix)/sbin/$(TARGET)

uninstall:
	-rm -f $(DESTDIR)$(prefix)/sbin/$(TARGET)

.PHONY: all clean install
