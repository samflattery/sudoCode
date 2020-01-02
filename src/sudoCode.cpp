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
 * generate_graph:
 * ---------------
 * output a graphviz graph of tree to a file at path file_s
 */
void generate_graph(INode tree, std::string file_s) {
	/* dot_file is the file without the 'png' prefix to which the dot prefix
	 * will be added so that a dot file can be created to feed to graphviz */
	std::string dot_file = file_s.substr(0, file_s.size() - 3);
	dot_file += "dot";
	tree.show_tree(dot_file.c_str()); /* generate the dot file */
	if (fork() == 0) {
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
	}
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		cerr << "usage: " << argv[0] << " <src.txt>" << endl;
		return 1;
	}

	bool graph_flag = false;
	int c;
	char *g_filepath = NULL;
	std::string file_s;

	while ((c = getopt(argc, argv, "hg:")) != -1) {
		switch (c) {
			case 'g':
				{ /* indent to define extension in this scope */
					graph_flag = true;
					g_filepath = optarg;
					file_s = g_filepath;
					std::string extension = file_s.substr(file_s.size() - 4);
					if (extension != ".png") {
						cerr << "file must be .png" << endl;
						return 1;
					}
					break;
				}
			case 'h':
				cout << "usage: " << argv[0] << " [options] <src.txt>" << endl;
				cout << "options: " << endl;
				cout << "-g <filepath>\tgenerate png of parsefile" << endl;
				break;
			case '?':
				if (optopt == 'g') {
					cerr << "usage: -g <filepath>" << endl;
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
	if (graph_flag) {
		generate_graph(tree, file_s);
	}
	auto ast = tree.to_ast();
	if (auto I = dynamic_cast<BinaryOperator*>(&*ast)) {
		I->show_tree("graph2.dot");
	}
	std::cout << ast->execute_node() << std::endl;
	return 0;

}
