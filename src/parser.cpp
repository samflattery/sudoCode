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
	shared_ptr<INode> P = make_shared<INode>(STMT);
	P->add_child(parse_exp_stmt());
	return P;
}

shared_ptr<PNode> Parser::parse_exp_stmt() {
	Token tok = peek();
	shared_ptr<INode> P = make_shared<INode>(EXP_STMT);
	shared_ptr<INode> test = make_shared<INode>(EXP_STMT);
	vector<shared_ptr<PNode>> children;
	shared_ptr<PNode> exp_child = parse_exp();
	if (exp_child == nullptr) {
		return nullptr;
	}
	shared_ptr<PNode> eol_child = parse_eol();
	if (eol_child == nullptr) {
		return nullptr;
	}
	children.push_back(exp_child);
	children.push_back(eol_child);
	P->set_children(children);
	return P;
}

/*
 ***************
 * experssions *
 ***************
 */
shared_ptr<PNode> Parser::parse_exp() {
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
			child = parse_arith_exp();
			break;
		default:
			return nullptr;
	}
	P->add_child(child);
	return P;
}

shared_ptr<PNode> Parser::parse_eol() {
	Token tok = next();
	return make_shared<LeafNode>(tok);
}

shared_ptr<PNode> Parser::parse_arith_exp() {
	shared_ptr<INode> P = make_shared<INode>(ARITH_EXP);
	shared_ptr<PNode> term = parse_term();
	if (term == nullptr) {
		return nullptr;
	}
	vector<shared_ptr<PNode> > children;
	children.push_back(term);
	if (tokens_empty()) {
		P->set_children(children);
		return P;
	}
	Token add = peek();
	while (add.kind == ADD_OP) {
		shared_ptr<PNode> add_op = parse_add_op();
		shared_ptr<PNode> term2 = parse_term();
		if (add_op == nullptr || term2 == nullptr) {
			return nullptr;
		}
		children.push_back(add_op);
		children.push_back(term2);
		if (tokens_empty()) {
			P->set_children(children);
			return P;
		}
		add = peek();
	}
	P->set_children(children);
	return P;
}

shared_ptr<PNode> Parser::parse_declr_exp() {
	shared_ptr<INode> P = make_shared<INode>(DECLRTN);
	vector<shared_ptr<PNode>> children;
	shared_ptr<PNode> declr = parse_token(DECLR);
	if (declr == nullptr) {
		return nullptr;
	}
	children.push_back(declr);
	shared_ptr<PNode> declrtr = parse_declarator();
	shared_ptr<PNode> assign = parse_token(ASSIGN);
	if (declrtr == nullptr || assign == nullptr) {
		return nullptr;
	}
	children.push_back(declrtr);
	children.push_back(assign);
	Token tok = peek();
	shared_ptr<PNode> child;
	if (tok.kind == ID || tok.kind == LITERAL) {
		child = parse_prim_exp();
	} else {
		child = parse_exp();
	}
	if (child == nullptr) {
		return nullptr;
	}
	children.push_back(child);
	P->set_children(children);
	return P;
}

shared_ptr<PNode> Parser::parse_assign_exp() {
	return nullptr;
}

shared_ptr<PNode> Parser::parse_prim_exp() {
	Token tok = peek();
	shared_ptr<INode> P = make_shared<INode>(PRIM_EXP);
	shared_ptr<PNode> child;
	switch (tok.kind) {
		case LITERAL:
			child = parse_constant();
			break;
		case ID:
			child = parse_identifier();
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

/*
 **************
 * operations *
 **************
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

shared_ptr<PNode> Parser::parse_mult_op() {
	Token tok = next();
	shared_ptr<LNode> P = make_shared<LNode>(tok);
	return P;
}

shared_ptr<PNode> Parser::parse_add_op() {
	Token tok = next();
	shared_ptr<LNode> P = make_shared<LNode>(tok);
	return P;
}

shared_ptr<PNode> Parser::parse_comp_op() {
	Token tok = next();
	shared_ptr<LNode> P = make_shared<LNode>(tok);
	return P;
}

/*
 **************
 * arithmetic *
 **************
 */
shared_ptr<PNode> Parser::parse_term() {
	shared_ptr<INode> P = make_shared<INode>(TERM);
	shared_ptr<PNode> factor = parse_factor();
	if (factor == nullptr) {
		return nullptr;
	}
	vector<shared_ptr<PNode>> children;
	children.push_back(factor);
	if (tokens_empty()) {
		P->set_children(children);
		return P;
	}
	Token mult = peek();
	while (mult.kind == MUL_OP) {
		shared_ptr<PNode> mult_op = parse_mult_op();
		shared_ptr<PNode> factor2 = parse_factor();
		if (mult_op == nullptr || factor2 == nullptr) {
			return nullptr;
		}
		children.push_back(mult_op);
		children.push_back(factor2);
		if (tokens_empty()) {
			P->set_children(children);
			return P;
		}
		mult = peek();
	}
	P->set_children(children);
	return P;
}

shared_ptr<PNode> Parser::parse_factor() {
	shared_ptr<INode> P = make_shared<INode>(FACTOR);
	shared_ptr<PNode> child = parse_prim_exp();
	if (child == nullptr) {
		return nullptr;
	}
	P->add_child(child);
	return P;
}

/*
 ****************
 * primary-exps *
 ****************
 */
shared_ptr<PNode> Parser::parse_identifier() {
	Token tok = next();
	shared_ptr<LNode> P = make_shared<LNode>(tok);
	return P;
}

shared_ptr<PNode> Parser::parse_constant() {
	Token tok = next();
	shared_ptr<LNode> P = make_shared<LNode>(tok);
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
	Token tok = m_tokens[m_tok_index];
	return tok;
}

bool inline Parser::tokens_empty() {
	return m_tok_index == m_tokens.size();
}
