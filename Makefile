CC      = gcc
CFLAGS  = -Wall -Wextra -std=c99 -O2 -D_POSIX_C_SOURCE=200809L
TARGET  = jogo_da_vida
SOURCE  = Jogo_da_Vida.c

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
