CC := gcc
CFLAGS := -Wall -Wextra -Werror -pedantic -Iin

TARGET := vshell
SOURCES := $(shell echo src/*.c)
HEADERS := $(shell echo src/*.h)
OBJECTS := $(SOURCES:.c=.o)

TEST_TARGET := test
TEST_SOURCES := $(shell echo tests/*.c) $(SOURCES)
TEST_HEADERS := $(shell echo tests/*.h) $(HEADERS)
TEST_OBJECTS := $(TEST_SOURCES:.c=.o) 

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

test: $(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJECTS)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(TEST_OBJECTS)

clean:
	-rm $(OBJECTS) $(TEST_OBJECTS)

%.o: %.c $(HEADERS) $(TEST_HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY : all test clean

