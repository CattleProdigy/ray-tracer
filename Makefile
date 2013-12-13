SHELL = /bin/bash

# Directories
OBJ_DIR		= ./obj
SRC_DIR		= ./src
TEST_DIR	= ./test
GTEST_DIR	= ./gtest
LIB_DIR		= ./lib

# Build Objects
SRCS		= $(wildcard $(SRC_DIR)/*.cpp)
INCLUDES 	= -I./include -I./ext_include  -I./ext_include/eigen3 -I./gtest/include -I./ext_include/assimp
TESTS 		= $(wildcard $(TEST_DIR)/*.cpp)

OBJS		= $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
NONMAINOBJS = $(filter-out ./obj/main.o,$(OBJS))

TEST_OBJS	= $(TESTS:$(TEST_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Compiler
CXX 		= g++
CPPFLAGS	= -isystem $(GTEST_DIR)/include
CXXFLAGS 	= -DWITH_MPI -fopenmp --std=c++11 -O3 -march=core2 -Wextra -Wall -pthread $(INCLUDES) $(LIBFLAGS) `libpng-config --cflags`
LIBFLAGS 	= -lassimp `libpng-config --ldflags`

# Google Test
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

.PHONY: clean all debug mpi

all: $(OBJS)

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

ray: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o ray 

mpi: CXX:=mpic++
mpi: ray 

mpi_debug: CXXFLAGS:=$(filter-out -O3,$(CXXFLAGS))
mpi_debug: CXXFLAGS += -g 
mpi_debug: mpi

debug: CXXFLAGS:=$(filter-out -O3,$(CXXFLAGS))
debug: CXXFLAGS += -g 
debug: ray


$(TEST_OBJS): $(OBJ_DIR)/%.o : $(TEST_DIR)/%.cpp \
				$(GTEST_HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

test: $(NONMAINOBJS) $(TEST_OBJS) ./obj/gtest_main.a
	echo $(NONMAINOBJS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o rt_test 
	./rt_test

clean:
	rm -rf *.o
	rm -rf ./obj/*
