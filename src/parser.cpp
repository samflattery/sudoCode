#include "parser.h"

InteriorNode Parser::generate_parse_tree() {
	vector<PNode*> children;
	while (!tokens_empty()) {
		children.push_back(parse_statement());
	}
	m_tree.set_children(children);
	return m_tree;
}

PNode *Parser::parse_exp_stmt() {
	Token tok = peek();
	INode *P = new INode(EXP_STMT);
	PNode *child = tok.kind == EOL ? parse_eol() : parse_exp();
	if (child == NULL) {
		delete P;
		return NULL;
	}
	P->add_child(child);
	return P;
}

PNode *Parser::parse_eol() {
	Token tok = next();
	return new LeafNode(tok);
}

PNode *Parser::parse_statement() {
	INode *P = new INode(STMT);
	P->add_child(parse_exp_stmt());
	return P;
}

PNode *Parser::parse_exp() {
	Token tok = peek();
	Kind kind = tok.kind;
	INode *P = new INode(EXP);
	PNode *child;
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
			delete P;
			return NULL;
	}
	P->add_child(child);
	return P;
}

PNode *Parser::parse_term() {
	INode *P = new INode(TERM);
	PNode *factor = parse_factor();
	if (factor == NULL) {
		delete P;
		return NULL;
	}
	vector<PNode*> children;
	children.push_back(factor);
	if (tokens_empty()) {
		P->set_children(children);
		return P;
	}
	Token mult = peek();
	while (mult.lexeme == "*" || mult.lexeme == "/" || mult.lexeme == "%") {
		PNode *mult_op = parse_mult_op();
		PNode *factor2 = parse_factor();
		if (mult_op == NULL || factor2 == NULL) {
			delete P;
			return NULL;
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

PNode *Parser::parse_factor() {
	INode *P = new INode(FACTOR);
	PNode *child = parse_prim_exp();
	if (child == NULL) {
		delete P;
		return NULL;
	}
	P->add_child(child);
	return P;
}

PNode *Parser::parse_arith_exp() {
	INode *P = new INode(ARITH_EXP);
	PNode *term = parse_term();
	if (term == NULL) {
		delete P;
		return NULL;
	}
	vector<PNode*> children;
	children.push_back(term);
	if (tokens_empty()) {
		P->set_children(children);
		return P;
	}
	Token add = peek();
	while (add.lexeme == "+" || add.lexeme == "-") {
		PNode *add_op = parse_add_op();
		PNode *term2 = parse_term();
		if (add_op == NULL || term2 == NULL) {
			delete P;
			return NULL;
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

PNode *Parser::parse_mult_op() {
	Token tok = next();
	LNode *P = new LNode(tok);
	return P;
}

PNode *Parser::parse_add_op() {
	Token tok = next();
	LNode *P = new LNode(tok);
	return P;
}

PNode *Parser::parse_prim_exp() {
	Token tok = peek();
	INode *P = new INode(PRIM_EXP);
	PNode *child;
	switch (tok.kind) {
		case LITERAL:
			child = parse_constant();
			break;
		case ID:
			child = parse_identifier();
			break;
		default:
			delete P;
			return NULL;
	}
	P->add_child(child);
	return P;
}

PNode *Parser::parse_identifier() {
	Token tok = next();
	LNode *P = new LNode(tok);
	return P;
}

PNode *Parser::parse_constant() {
	Token tok = next();
	LNode *P = new LNode(tok);
	return P;
}

PNode *Parser::parse_declr_exp() {
	return NULL;
}

PNode *Parser::parse_assign_exp() {
	return NULL;

}

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
