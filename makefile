CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -lpthread -lrt -lglut -lGLU -lGL -lm

TARGET = main
SOURCES = main.c
OBJECTS = $(SOURCES:.c=.o)

.PHONY: all debug clean

all: $(TARGET)

debug: CFLAGS += -g -DDEBUG
debug: $(TARGET)

$(TARGET): $(OBJECTS)
	@echo "Linking target: $(TARGET)"
	@$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) -o $@

%.o: %.c
	@echo "Compiling file: $<"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning..."
	@rm -f $(OBJECTS) $(TARGET)
