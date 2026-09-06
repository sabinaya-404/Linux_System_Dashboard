CC = gcc
CFLAGS = -Wall -Wextra -O2 -Iinclude
SRC = src/main.c src/collector/memory.c
SRC = src/main.c src/collector/memory.c src/collector/cpu.c
TARGET = sysmon

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean
