#include "parser.h"

ParseTree Parser::generate_parse_tree() {
	parse_tokens();
	return m_tree;
}

bool Parser::parse_tokens() {
	while (!tokens_empty()) {
		parse_expression();
	}
	return true;
}

bool Parser::parse_expression() {
	Token tok = next_token();
	Kind kind = tok.kind;
	switch (kind) {
		case EOL:
			return true;
		case DECLR:
			parse_declaration();
		case ASSIGN:
			parse_assignment();
	}
}

PNode *Parser::parse_declaration() {
	Token tok = next_token();
	/* m_tree.add_node(tok); */
	/* parse_qualified_name(); */
	/* parse_type(); */
	/* parse_called(); */
	/* parse_id(); */
	/* parse_assign(); */
	/* parse_int(); */
	return NULL;
}

PNode *Parser::parse_assignment() {
	return NULL;

}

Token inline Parser::next_token() {
	Token tok = m_tokens[m_tok_index];
	m_tok_index++;
	return tok;
}

bool inline Parser::tokens_empty() {
	return m_tok_index == m_tokens.size();
}
