CC = g++
CFLAGS = -std=c++11 -Wall -O2 -static
TARGET = nova
SRC = src/cpp/nova.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

install: $(TARGET)
	cp $(TARGET) /data/data/com.termux/files/usr/bin/
	chmod +x /data/data/com.termux/files/usr/bin/$(TARGET)

uninstall:
	rm -f /data/data/com.termux/files/usr/bin/$(TARGET)

.PHONY: all clean install uninstall
