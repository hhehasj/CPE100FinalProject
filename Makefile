# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -lm

# Target executable
TARGET = calculus_learning

# Source files
SRCS = main.c question_loader.c teaching_loader.c quiz_engine.c teaching_renderer.c level_rules.c progress_persistence.c

# Object files
OBJS = $(SRCS:.c=.o)

# Header files
HEADERS = structures.h question_loader.h teaching_loader.h quiz_engine.h teaching_renderer.h level_rules.h progress_persistence.h

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(CFLAGS)

# Compile source files to object files
%.o: %.c $(HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS)

# Clean build files
clean:
	rm -f $(OBJS) $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Rebuild everything
rebuild: clean all

.PHONY: all clean run rebuild