#include "lexer.h"

bool Lexer::is_number(string& s) {
	string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

void Lexer::register_types() {
	vector<struct state> states =	{	{ "a whole number", INT, "" },
										{ "a sequence of characters", STRING, ""},
										{ "a character", CHAR, "" }
									};
	for (auto st : states) {
		m_states.push_back(st);
	}
}

void Lexer::register_operators() {
	vector<struct state> states =	{	{ "added to", ARITH_OP, "+" },
										{ "subtracted from", ARITH_OP, "-" },
										{ "multiplied by", ARITH_OP, "*" },
										{ "divided by", ARITH_OP, "/"},
										{ "modulo", ARITH_OP, "%" }
									};
	for (auto st : states) {
		m_states.push_back(st);
	}
}

void Lexer::register_declarations() {
	vector<struct state> states =	{	{ "declare that", DECLR, "" },
										{ "called", CALLD, "" },
										{ "store the value of", ASSIGN, "" },
										{ "in", IN, "" },
										{ "exists and instantiate it to the "
										  "value of", ASSIGN, "" },
									};
	for (auto st : states) {
		m_states.push_back(st);
	}
}

void Lexer::register_eol() {
	vector<struct state> states = {{ "and then end this line", EOL, "" }};
	for (auto st : states) {
		m_states.push_back(st);
	}
}

void Lexer::register_states() {
	register_types();
	register_operators();
	register_declarations();
	register_eol();
	m_dfa.register_states(m_states);
}

bool Lexer::parse_word() {
	std::string word;
	while (*m_str != '\0' && !isspace(*m_str)) {
		word += *m_str;
		m_str++;
	}
	Token tok;
	is_number(word) ?  tok = { LITERAL, word } : tok = { ID, word };
	m_tokens.push_back(tok);
	return true;
}

vector<Token> Lexer::tokenize() {
	register_states();
	Token tok;
	while (*m_str) {
		if (m_dfa.recognize_string(m_str, tok)) {
			m_tokens.push_back(tok);
		} else {
			parse_word();
		}
		m_str++;
	}
	return m_tokens;
}
