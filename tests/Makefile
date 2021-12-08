HOME     = ../buffer/
BINARIES = ./

SOURCES = \
	test.cpp\
	$(HOME)buffer.cpp

OBJECTS = $(SOURCES:%.cpp=%.o)
CXX = g++ -std=c++11

INCLUDES = -I$(HOME)

CXXFLAGS = -Wall -Wextra

LDFLAGS = `pkg-config --libs --cflags gtest`

GTEST = /usr/src/gtest/lib/libgtest.a

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

default: tests

tests: $(OBJECTS)
	$(CXX) -o $(BINARIES)$@ $(OBJECTS) $(LDFLAGS) $(GTEST)

clean:
	/bin/rm -f $(OBJECTS) $(BINARIES)tests
