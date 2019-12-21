#ifndef __PARSER_H__
#define __PARSER_H__

#include <iostream>
#include <string>
#include "lexer.h"
#include "tokens.h"
#include "parsetree.h"

using std::vector;
typedef ParseTreeNode PNode;

class Parser {
public:
	Parser(vector<Token>& tokens) : m_tokens(tokens) {}
	ParseTree *generate_parse_tree();
private:
	vector<Token> m_tokens;
	ParseTree *m_tree;
	bool parse_tokens();
};

#endif /* __PARSER_H__ */
