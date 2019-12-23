#ifndef __PARSER_H__
#define __PARSER_H__

#include <iostream>
#include <string>
#include <memory>
#include "lexer.h"
#include "tokens.h"
#include "parsetree.h"
#include "grammar.h"

using std::vector;
using std::shared_ptr;
using std::make_shared;
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
	shared_ptr<PNode> parse_statement();
	shared_ptr<PNode> parse_exp_stmt();

	/* expressions */
	shared_ptr<PNode> parse_exp();
	shared_ptr<PNode> parse_assign_exp();
	shared_ptr<PNode> parse_arith_exp();
	shared_ptr<PNode> parse_declr_exp();
	shared_ptr<PNode> parse_prim_exp();
	shared_ptr<PNode> parse_eol();

	/* operators */
	shared_ptr<PNode> parse_mult_op();
	shared_ptr<PNode> parse_add_op();

	/* arithmetic */
	shared_ptr<PNode> parse_term();
	shared_ptr<PNode> parse_factor();

	/* other */
	shared_ptr<PNode> parse_identifier();
	shared_ptr<PNode> parse_constant();
};

#endif /* __PARSER_H__ */
