CXX = g++
TARGET = genetic
OUT_DIR = build
LIBS = -Wall -Wextra -lm -lpthread -std=c++14
OPTIMIZATION = -O2
DEBUG = -g0
CXX_FLAGS = $(LIBS) $(OPTIMIZATION) $(DEBUG)

.PHONY: all clean

all: $(OUT_DIR)/$(TARGET)

$(OUT_DIR)/$(TARGET): $(OUT_DIR)/main.o
	$(CXX) $(OUT_DIR)/main.o $(CXX_FLAGS) -o $(OUT_DIR)/$(TARGET)

$(OUT_DIR)/main.o: main.cpp *.h
	mkdir -p $(OUT_DIR)
	$(CXX) main.cpp $(CXX_FLAGS) -c -o $(OUT_DIR)/main.o

clean:
	rm -rf $(OUT_DIR)
