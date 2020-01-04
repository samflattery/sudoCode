/*
 * parser.cpp
 * ----------
 * implements the Parser class
 */

#include "parser.h"

/*
 * generate_parse_tree:
 * --------------------
 * transforms m_tokens into a parse tree
 */
InteriorNode Parser::generate_parse_tree() {
	vector<shared_ptr<PNode> > children;
	while (!tokens_empty()) {
		children.push_back(parse_statement());
	}
	m_tree.set_children(children);
	return m_tree;
}

/*
 **************
 * statements *
 **************
 */
shared_ptr<PNode> Parser::parse_statement() {
	/*
	<statement> ::= <expression-statement>
				  | <if-statement>
				  | <iteration-statement>
				  | <jump-statement>
				  | <compound-statement>
	*/
	shared_ptr<INode> P = make_shared<INode>(STMT);
	P->add_child(parse_exp_stmt());
	return P;
}

shared_ptr<PNode> Parser::parse_exp_stmt() {
	/* <expression-statement> ::= (<expression>)? <eol> */
	Token tok = peek();
	shared_ptr<INode> P = make_shared<INode>(EXP_STMT);
	if (tok.kind == EOL) {
		shared_ptr<PNode> eol_child = parse_eol();
		if (eol_child == nullptr) {
			return nullptr;
		}
		P->add_child(eol_child);
		return P;
	}
	shared_ptr<PNode> exp_child = parse_exp();
	if (exp_child == nullptr) {
		return nullptr;
	}
	shared_ptr<PNode> eol_child = parse_eol();
	if (eol_child == nullptr) {
		return nullptr;
	}
	P->set_children( { exp_child, eol_child });
	return P;
}

/*
 ***************
 * experssions *
 ***************
 */
shared_ptr<PNode> Parser::parse_exp() {
	/*
	<expression> ::= <assignment-expression>
				   | <arithmetic-expression>
				   | <declaration-expression>
	*/
	Token tok = peek();
	Kind kind = tok.kind;
	shared_ptr<INode> P = make_shared<INode>(EXP);
	shared_ptr<PNode> child;
	switch (kind) {
		case EOL:
			child = parse_eol();
			break;
		case DECLR:
			child = parse_declr_exp();
			break;
		case ASSIGN:
			child = parse_assign_exp();
			break;
		case LITERAL:
		case LPAREN:
		case RPAREN:
		case ID:
			child = parse_add_exp();
			break;
		default:
			return nullptr;
	}
	P->add_child(child);
	return P;
}

shared_ptr<PNode> Parser::parse_eol() {
	return parse_token(EOL);
}


shared_ptr<PNode> Parser::parse_declr_exp() {
	/*
	<declaration-expression> ::= declare that <declarator> exists and
	 							 <assignment-expression>
	 */
	shared_ptr<INode> P = make_shared<INode>(DECLRTN);
	vector<shared_ptr<PNode>> children;
	shared_ptr<PNode> declr = parse_token(DECLR);
	if (declr == nullptr) {
		return nullptr;
	}
	shared_ptr<PNode> declrtr = parse_declarator();
	if (declrtr == nullptr) {
		return nullptr;
	}
	shared_ptr<PNode> exists = parse_token(EXIST);
	if (exists == nullptr) {
		return nullptr;
	}
	shared_ptr<PNode> assign_exp = parse_assign_exp();
	if (assign_exp == nullptr) {
		return nullptr;
	}
	P->set_children( { declr, declrtr, exists, assign_exp } );
	return P;
}

shared_ptr<PNode> Parser::parse_assign_exp() {
	/*
	<assignment-expression> ::= <assigner> ( <expression> |
														<primary-expression> )
    */
	shared_ptr<INode> P = make_shared<INode>(ASSIGN_EXP);
	shared_ptr<PNode> phrase = parse_assigner();
	if (phrase == NULL) {
		return nullptr;
	}
	shared_ptr<PNode> expr;
	if ((expr = parse_exp()) == nullptr) {
		expr = parse_prim_exp();
	}
	if (expr == nullptr) {
		return nullptr;
	}
	P->set_children( {phrase, expr} );
	return P;
}


shared_ptr<PNode> Parser::parse_prim_exp() {
	/*
	<primary-expression> ::= <identifier>
						   | <constant>
	*/
	Token tok = peek();
	shared_ptr<INode> P = make_shared<INode>(PRIM_EXP);
	shared_ptr<PNode> child;
	switch (tok.kind) {
		case LITERAL:
			child = parse_token(LITERAL);
			break;
		case ID:
			child = parse_token(ID);
			break;
		default:
			return nullptr;
	}
	P->add_child(child);
	return P;
}

/*
 ********************
 * declr/assign exp *
 ********************
 */
shared_ptr<PNode> Parser::parse_declarator() {
	/* <declarator> ::= <type-specifier> called <identifier> */
	shared_ptr<INode> P = make_shared<INode>(DECLRTR);
	shared_ptr<PNode> type = parse_token(TYPE);
	if (type == nullptr) {
		return nullptr;
	}
	shared_ptr<PNode> calld = parse_token(CALLD);
	if (calld == nullptr) {
		return nullptr;
	}
	shared_ptr<PNode> id = parse_token(ID);
	if (id == nullptr) {
		return nullptr;
	}
	vector<shared_ptr<PNode>> children = { type, calld, id };
	P->set_children(children);
	return P;
}

shared_ptr<PNode> Parser::parse_assigner() {
	/* <assigner> ::= assign <identifier> the value of */
	shared_ptr<INode> P = make_shared<INode>(ASSIGNR);
	shared_ptr<PNode> assgn = parse_token(ASSIGN);
	shared_ptr<PNode> id = parse_token(ID);
	shared_ptr<PNode> val = parse_token(VALUE);
	if (assgn == nullptr || id == nullptr || val == nullptr) {
		return nullptr;
	}
	P->set_children( { assgn, id, val } );
	return P;
}


/*
 **************
 * operations *
 **************
 */

/*
 * parse_token:
 * ------------
 * tries to create a new leaf of Kind k
 * only modifies the token index if successfully finds the right kind
 */
shared_ptr<PNode> Parser::parse_token(Kind k) {
	Token tok = next();
	if (tok.kind != k) {
		prev();
		return nullptr;
	}
	shared_ptr<LNode> P = make_shared<LNode>(tok);
	return P;
}

/*
 **************
 * arithmetic *
 **************
 */

shared_ptr<PNode> Parser::parse_add_exp() {
	/*
	<additive-expression> ::= <multiplicative-expression>
	 					(<additive-operator> <multiplicative-expression>)?
	*/
	shared_ptr<INode> P = make_shared<INode>(ADD_EXP);
	shared_ptr<PNode> term = parse_mult_exp();
	if (term == nullptr) {
		return nullptr;
	}
	vector<shared_ptr<PNode>> children;
	children.push_back(term);
	bool first = true;
	Token add = peek();
	while (add.kind == ADD_OP) {
		shared_ptr<PNode> add_op = parse_token(ADD_OP);
		shared_ptr<PNode> term2 = parse_mult_exp();
		if (add_op == nullptr || term2 == nullptr) {
			return nullptr;
		}
		shared_ptr<INode> n = make_shared<INode>(ADD_EXP);
		vector<shared_ptr<PNode>> children2;
		if (first) {
			children2.push_back(term);
			first = false;
		} else {
			children2.push_back(P);
		}
		children2.push_back(add_op);
		children2.push_back(term2);
		n->set_children(children2);
		P = n;
		add = peek();
	}
	if (first) {
		P->set_children(children);
	}
	return P;
}

shared_ptr<PNode> Parser::parse_mult_exp() {
	/*
	<multiplicative-expression> ::= <factor>
	 					(<multiplicative-operator> <factor>)?
	*/
	shared_ptr<INode> P = make_shared<INode>(MULT_EXP);
	shared_ptr<PNode> factor = parse_factor();
	if (factor == nullptr) {
		return nullptr;
	}
	vector<shared_ptr<PNode>> children;
	children.push_back(factor);
	bool first = true;
	Token mult = peek();
	while (mult.kind == MUL_OP) {
		shared_ptr<PNode> mult_op = parse_token(MUL_OP);
		shared_ptr<PNode> factor2 = parse_factor();
		if (mult_op == nullptr || factor2 == nullptr) {
			return nullptr;
		}
		vector<shared_ptr<PNode>> children2;
		shared_ptr<INode> n = make_shared<INode>(MULT_EXP);
		if (first) {
			children2.push_back(factor);
			first = false;
		} else {
			children2.push_back(P);

		}
		children2.push_back(mult_op);
		children2.push_back(factor2);
		n->set_children(children2);
		P = n;
		mult = peek();
	}
	if (first) {
		P->set_children(children);
	}
	return P;
}

shared_ptr<PNode> Parser::parse_factor() {
	/* <factor> ::= <primary-expression> | "(" <expr> ")" */
	shared_ptr<INode> P = make_shared<INode>(FACTOR);
	Token tok = peek();
	if (tok.kind == LPAREN) {
		shared_ptr<PNode> L = parse_token(LPAREN);
		shared_ptr<PNode> expr = parse_exp();
		shared_ptr<PNode> R = parse_token(RPAREN);
		P->set_children({L, expr, R});
		return P;
	}
	shared_ptr<PNode> child = parse_prim_exp();
	if (child == nullptr) {
		return nullptr;
	}
	P->add_child(child);
	return P;
}

/*
 **********************
 * state manipulation *
 **********************
 */
Token inline Parser::next() {
	Token tok = m_tokens[m_tok_index];
	m_tok_index++;
	return tok;
}

Token inline Parser::prev() {
	Token tok = m_tokens[m_tok_index];
	m_tok_index--;
	return tok;
}

Token inline Parser::peek() {
	if (tokens_empty()) {
		return { END, "" };
	}
	Token tok = m_tokens[m_tok_index];
	return tok;
}

bool inline Parser::tokens_empty() {
	return m_tok_index == m_tokens.size();
}
