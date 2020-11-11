CC := gcc
CFLAGS := -Wall -Wextra -Werror -g -pedantic -I"lib" -I"src"

TARGET := vshell
SOURCES := $(wildcard src/*.c)
HEADERS := $(wildcard src/*.h)
OBJECTS := $(SOURCES:.c=.o)

TEST_TARGET := test
TEST_SOURCES := tests/test.c 

TEST_LIB_SOURCES := lib/munit/munit.c
TEST_LIB_HEADERS := lib/munit/munit.h
TEST_LIB_OBJECTS := $(patsubst %.c,%.o,$(TEST_LIB_SOURCES))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJECTS)


$(TEST_TARGET): $(TEST_SOURCES) $(TEST_LIB_OBJECTS) \
	$(filter-out src/vshell.o,$(OBJECTS))
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(TEST_LIB_OBJECTS) \
		$(TEST_SOURCES) $(filter-out src/vshell.o,$(OBJECTS))

clean:
	-rm $(OBJECTS) $(TEST_OBJECTS) 

cleanall: clean
	-rm  $(TARGET) $(TEST_TARGET)


%.o: %.c $(HEADERS) 
	$(CC) $(CFLAGS) -I"src" -c -o $@ $<

.PHONY : all test clean cleanall

