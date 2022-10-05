CXX      = g++ -std=c++11
CXXFLAGS = -Wall -Wextra
LDFLAGS  = `pkg-config --libs --cflags gtest`
GTEST    = /usr/src/gtest/lib/libgtest.a


all : buffer-tests list-tests

buffer-tests : buffer.o buffer_tests.o
	$(CXX) -o buffer-tests buffer.o buffer_tests.o $(LDFLAGS) $(GTEST)

list-tests : list_tests.o
	$(CXX) -o list-tests list_tests.o $(LDFLAGS) $(GTEST)

buffer.o : ../buffer/buffer.cpp ../buffer/buffer.hpp
	$(CXX) $(CXXFLAGS) -c ../buffer/buffer.cpp 

buffer_tests.o : buffer_tests.cpp ../buffer/buffer.hpp
	$(CXX) $(CXXFLAGS) -c buffer_tests.cpp

list_tests.o : list_tests.cpp ../list/list.hpp
	$(CXX) $(CXXFLAGS) -c list_tests.cpp

clean :
	rm buffer.o buffer_tests.o list_tests.o buffer-tests list-tests