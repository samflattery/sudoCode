#include "parser.h"

ParseTree *Parser::generate_parse_tree() {
	PNode *node = new PNode( { MAIN, ""} );
	ParseTree *P = new ParseTree(node);
	m_tree = P;
	parse_tokens();
	return m_tree;
}
