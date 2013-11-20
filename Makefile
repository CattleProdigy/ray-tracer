SHELL = /bin/bash

# Directories
OBJ_DIR		= ./obj
SRC_DIR		= ./src
TEST_DIR	= ./test
GTEST_DIR	= ./gtest

# Build Objects
SRCS		= $(wildcard $(SRC_DIR)/*.cpp)
INCLUDES 	= -I./include -I/usr/include/eigen3 -I./gtest/include
TESTS 		= $(wildcard $(TEST_DIR)/*.cpp)

OBJS		= $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TEST_OBJS	= $(TESTS:$(TEST_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Compiler
CXX 		= g++
CPPFLAGS	= -isystem $(GTEST_DIR)/include
CXXFLAGS 	= -Wextra -Wall -pthread $(INCLUDES) `libpng-config --cflags`
LIBFLAGS 	= `libpng-config --ldflags`

# Google Test
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

$(OBJ_DIR)/gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc -o $@

$(OBJ_DIR)/gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc -o $@

$(OBJ_DIR)/gtest.a : ./obj/gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

$(OBJ_DIR)/gtest_main.a : ./obj/gtest-all.o ./obj/gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

# Targets
$(OBJS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

image_test: $(OBJS)
	$(CXX) $(OBJS) $(LIBFLAGS) -o image_test

$(TEST_OBJS): $(OBJ_DIR)/%.o : $(TEST_DIR)/%.cpp \
				$(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

test: obj/color.o obj/sphere.o obj/ray.o $(TEST_OBJS) ./obj/gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o rt_test 
	./rt_test

debug:
	echo $(TEST_OBJS)
	echo $(OBJS)
	echo $(AR)
	echo $(ARFLAGS)
	echo $(SRCS)

.PHONY: clean

clean:
	rm -rf *.o
	rm -rf ./obj/*
