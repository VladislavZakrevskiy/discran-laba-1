CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -Wpedantic -Iinclude

TARGET := solution
BENCH_TARGET := bench
OBJ_DIR := .obj

SRCS := src/contest_main.cpp
OBJS := $(SRCS:src/%.cpp=$(OBJ_DIR)/%.o)

BENCH_SRCS := src/bench.cpp
BENCH_OBJS := $(BENCH_SRCS:src/%.cpp=$(OBJ_DIR)/%.o)

.PHONY: all clean test bench

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@

bench: $(BENCH_OBJS)
	$(CXX) $(BENCH_OBJS) -o $(BENCH_TARGET)

$(OBJ_DIR)/%.o: src/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET) $(BENCH_TARGET)

test: $(TARGET)
	bash tests/test_contest.sh
