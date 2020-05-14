/*
 * sudoCode.cpp
 * ------------
 * runs the lexer and parser to create a parse tree
 */

#include "sudoCode.h"

using std::endl;
using std::cout;
using std::cerr;

/*
 * generate_png:
 * -------------
 * output a graphviz graph of tree to a file at path file_s
 */
void generate_png(std::string file_s, const char *dot_file) {
	/* dot_file is the file without the 'png' prefix to which the dot prefix
	 * will be added so that a dot file can be created to feed to graphviz */
	int child;
	if ((child = fork()) == 0) {
		std::string cmd = "dot";
		std::string arg0 = "-Tpng";
		std::string arg1 = dot_file;
		std::string arg2 = "-o" + file_s;
		char* const args[] = { &cmd[0], &arg0[0], &arg1[0], &arg2[0], NULL };
		if (execvp(args[0], args) < 0) { /* run the graphviz program */
			cerr << "error generating graph: ensure you have graphviz installed"
			     << " (brew install graphviz)" << endl;
			exit(1);
		}
	} else {
		waitpid(child, NULL, 0);
		remove(dot_file);
	}
}

/*
 * get_dot_path:
 * -------------
 * converts <file>.png to <file>.dot
 */
std::string get_dot_path(std::string file_s) {
	std::string extension = file_s.substr(file_s.size() - 4);
	if (extension != ".png") {
		cerr << "file must be .png" << endl;
		exit(1);
	}
	std::string dot_file = file_s.substr(0, file_s.size() - 3);
	dot_file += "dot";
	return dot_file;
}

/*
 * generate_graph:
 * ---------------
 * generates a dot file for either an AST or a ParseTree
 */
/* void generate_graph(INode tree, std::string file_s) { */
/* 	const char *dot = get_dot_path(file_s); */
/* 	tree.show_tree(dot); /1* generate the dot file *1/ */
/* 	generate_png(file_s, dot); */
/* } */

void generate_graph(Tree& tree, std::string file_s) {
	std::string dot = get_dot_path(file_s);
	tree.show_tree(&dot[0]);
	generate_png(file_s, &dot[0]);
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		cerr << "usage: " << argv[0] << " <src.txt>" << endl;
		return 1;
	}

	bool parse_flag = false;
	bool ast_flag = false;
	int c;
	std::string parse_file_s;
	std::string ast_file_s;

	while ((c = getopt(argc, argv, "hp:a:")) != -1) {
		switch (c) {
			case 'p':
				parse_flag = true;
				parse_file_s = optarg;
				break;
			case 'a':
				ast_flag = true;
				ast_file_s = optarg;
				break;
			case 'h':
				cout << "usage: " << argv[0] << " [options] <src.txt>" << endl;
				cout << "options: " << endl;
				cout << "-g <filepath>\tgenerate png of parsefile" << endl;
				break;
			case '?':
				if (optopt == 'p') {
					cerr << "usage: -p <filepath>" << endl;
				} else if (optopt == 'a') {
					cerr << "usage: -p <filepath>" << endl;
				} else {
					cerr << "unknown option " << (char) optopt << endl;
				}
				return 1;
			default:
				cerr << "invalid flag, use -h to see options" << endl;
				return 1;
		}
	}

	char *filename = argv[argc-1];
	Reader r(filename);

	size_t length;
	const char *src = r.read_file(length);
	if (src == NULL) {
		cout << "unable to read file: " << filename << endl;
		return 1;
	}

	Lexer lexer(src);
	vector<struct Token> tokens = lexer.tokenize();

	for (auto tok : tokens) {
		cout << tok << endl;
	}

	Parser parser(tokens);

	auto tree = parser.generate_parse_tree();
	if (parse_flag) {
		generate_graph(tree, parse_file_s);
	}

	auto ast = tree.to_ast();
	if (ast_flag) {
		if (auto I = dynamic_cast<MainFunction*>(&*ast)) {
			generate_graph(*I, ast_file_s);
		}
	}

	std::cout << std::get<int>(ast->execute_node()) << std::endl;
	return 0;

}
