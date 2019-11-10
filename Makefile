# gtest makefile by Ákos Becsey

CFLAGS = -Wall -std=c++11 -ggdb
LIBS = -lgtest_main -lgtest -lpthread 

CC = g++
LD = $(CC)

SRC_DIR   = fparser
BUILD_DIR = build
BIN_DIR   = bin
TARGET = tests

default : $(TARGET)
all : default
cross : default

#find sources
SOURCES = $(shell find $(SRC_DIR) -name "*.cpp")
OBJECTS = $(subst $(SRC_DIR),$(BUILD_DIR),$(SOURCES))
OBJECTS := $(OBJECTS:.cpp=.o)
OBJECTS := $(OBJECTS) $(BUILD_DIR)/tests.o

$(BUILD_DIR)/tests.o : tests.cpp
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ -Wall

# compile
$(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ -Wall

# link
$(TARGET) : $(OBJECTS)
	mkdir -p $(BIN_DIR)
	$(LD) $(OBJECTS) $(LIBS) -o $(BIN_DIR)/$@


RM = rm -f
clean:
	$(RM) -r $(BUILD_DIR)/*
	$(RM) $(BIN_DIR)/$(TARGET)
	$(RM) -r $(BUILD_DIR)
	$(RM) -r $(BIN_DIR)

test:
	./bin/tests

print-% : ; @echo $* = $($*)