SHELL = /bin/bash
ODIR   = ./obj
SDIR   = ./src
SRCS = $(wildcard $(SDIR)/*.cpp)
OBJS = $(SRCS:$(SDIR)/%.cpp=$(ODIR)/%.o)
INCLUDES = -I./include -I/usr/include/eigen3
CXX = g++
CXXFLAGS = --std=c++11 -Wall $(INCLUDES) `libpng-config --cflags`

$(OBJS): $(ODIR)/%.o : $(SDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

image_test: $(OBJS)
	$(CXX) $(OBJS) `libpng-config --ldflags` -o image_test

debug:
	echo $(ODIR)/%.o
	echo $(OBJ)
	echo $(SDIR)/*.cpp
