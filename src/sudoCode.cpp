/*
 * sudoCode.cpp
 * ------------
 * runs the lexer and parser to create a parse tree
 */

#include "sudoCode.h"

int main(int argc, char *argv[]) {
	if (argc < 2) {
		std::cout << "usage: " << argv[0] << " <src.txt>" << std::endl;
		return 1;
	}

	char *filename = argv[1];
	Reader r(filename);

	size_t length;
	const char *c = r.read_file(length);
	if (c == NULL) {
		std::cout << "unable to read file: " << filename << std::endl;
		return 1;
	}

	Lexer lexer(c);
	vector<struct Token> tokens = lexer.tokenize();

	for (auto tok : tokens) {
		std::cout << tok;
	}

	Parser parser(tokens);

	auto tree = parser.generate_parse_tree();
	tree.traverse(0);
	return 0;

}
