#
# Makefile for sudoCode
# CXXFLAGS = -g -Og -Wall -Wc++11-extensions -std=c++17 -stdlib=libc++
#

CC = gcc
CXX = g++-9
CXXFLAGS = -g -Og -Wall -std=c++17
DEPS = lexer.hpp dfa.hpp tokens.hpp reader.hpp
OBJDIR = build
BINDIR = bin
vpath %.hpp src
vpath %.cpp src
OBJ = $(addprefix $(OBJDIR)/, lexer.o dfa.o reader.o)

# Default build rule
.PHONY: all
all: lexer

lexer: $(OBJ)
	$(CXX) $(CXXFLAGS) -o lexer $(OBJ)

$(OBJDIR)/%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o lexer a.out
