# Make sudoCode

CC = gcc
CXX = g++-9
CXXFLAGS = -g -Og -Wall -std=c++17
DEPS = sudoCode.h parser.h lexer.h dfa.h tokens.h reader.h parsetree.h grammar.h
OBJDIR = build
vpath %.h src
vpath %.cpp src
OBJ = $(addprefix $(OBJDIR)/, sudoCode.o parser.o lexer.o dfa.o reader.o tokens.o parsetree.o grammar.o)

# Default build rule
.PHONY: all
all: sudoCode

sudoCode: $(OBJ)
	$(CXX) $(CXXFLAGS) -o sudoCode $(OBJ)

$(OBJDIR)/%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o sudoCode a.out
