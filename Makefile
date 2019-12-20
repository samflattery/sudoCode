#
# Makefile for sudoCode
# CXXFLAGS = -g -Og -Wall -Wc++11-extensions -std=c++17 -stdlib=libc++
#

CC = gcc
CXX = g++-9
CXXFLAGS = -g -Og -Wall -std=c++17
DEPS = lexer.hpp dfa.hpp tokens.hpp reader.hpp
OBJ = lexer.o dfa.o

# Default build rule
.PHONY: all
all: lexer

lexer: lexer.o dfa.o reader.o
	$(CXX) $(CXXFLAGS) -o lexer lexer.o dfa.o reader.o

lexer.o: lexer.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c lexer.cpp

dfa.o: dfa.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c dfa.cpp

reader.o: reader.cpp reader.hpp
	$(CXX) $(CXXFLAGS) -c reader.cpp

.PHONY: clean
clean:
	rm -f *.o lexer a.out
