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
	Parser(vector<Token>& tokens) :
		m_tokens(tokens),
		m_tree(ParseTree(PNode({ MAIN, "" }))) { }

	ParseTree generate_parse_tree();
private:
	vector<Token> m_tokens;
	size_t m_tok_index;

	ParseTree m_tree;
	bool parse_tokens();
	bool parse_expression();
	Token inline next_token();
	bool tokens_empty();

	PNode *parse_declaration();
	PNode *parse_assignment();
};

#endif /* __PARSER_H__ */
