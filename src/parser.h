#ifndef __PARSER_H__
#define __PARSER_H__

#include <iostream>
#include <string>
#include "lexer.h"
#include "tokens.h"
#include "parsetree.h"
#include "grammar.h"

using std::vector;
typedef ParseTreeNode PNode;
typedef InteriorNode INode;
typedef LeafNode LNode;

class Parser {
public:
	Parser(vector<Token>& tokens) :
		m_tokens(tokens),
		m_tok_index(0),
		m_tree(MAIN_FUN) { }

	InteriorNode generate_parse_tree();
private:
	vector<Token> m_tokens;
	size_t m_tok_index;

	InteriorNode m_tree;
	Token inline next();
	Token inline peek();
	Token inline prev();
	bool tokens_empty();

	/* statements */
	PNode *parse_statement();
	PNode *parse_exp_stmt();

	/* expressions */
	PNode *parse_exp();
	PNode *parse_assign_exp();
	PNode *parse_arith_exp();
	PNode *parse_declr_exp();
	PNode *parse_prim_exp();
	PNode *parse_eol();

	/* operators */
	PNode *parse_mult_op();
	PNode *parse_add_op();

	/* arithmetic */
	PNode *parse_term();
	PNode *parse_factor();

	/* other */
	PNode *parse_identifier();
	PNode *parse_constant();
};

#endif /* __PARSER_H__ */
