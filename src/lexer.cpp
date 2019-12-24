/*
 * lexer.cpp
 * ---------
 * implements the Lexer class
 */

#include "lexer.h"

/*
 * is_number:
 * ----------
 * return true iff the input string is numeric
 */
bool Lexer::is_number(string& s) {
	string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

/*
 * register functions:
 * -------------------
 * register various tokens with the dfa
 */
void Lexer::register_types() {
	vector<struct state> states =	{	{ "a whole number", TYPE, "INT" },
										{ "a sequence of characters", TYPE, "STRING"},
										{ "a character", TYPE, "CHAR" }
									};
	for (auto st : states) {
		m_states.push_back(st);
	}
}

void Lexer::register_operators() {
	vector<struct state> states =	{	{ "added to", ADD_OP, "+" },
										{ "subtracted from",  ADD_OP, "-" },
										{ "multiplied by", MUL_OP, "*" },
										{ "divided by", MUL_OP, "/"},
										{ "modulo", MUL_OP, "%" }
									};
	for (auto st : states) {
		m_states.push_back(st);
	}
}

void Lexer::register_declarations() {
	vector<struct state> states =	{	{ "declare that", DECLR, "" },
										{ "called", CALLD, "" },
										{ "assign", ASSIGN, "" },
										{ "the value of", VALUE, "" },
										{ "exists and", EXIST, "" },
									};
	for (auto st : states) {
		m_states.push_back(st);
	}
}

void Lexer::register_eol() {
	/* vector<struct state> states = {{ "and then end this line", EOL, "" }}; */
	/* for (auto st : states) { */
	/* 	m_states.push_back(st); */
	/* } */
}

void Lexer::register_states() {
	register_types();
	register_operators();
	register_declarations();
	register_eol();
	m_dfa.register_states(m_states);
}

/*
 * parse_word:
 * -----------
 * parses a word as an identifier or constant
 * returns true iff successfully parsed the word and modifies the class token
 * vector
 */
bool Lexer::parse_word() {
	std::string word;
	while (*m_str != '\0' && !isspace(*m_str)) {
		word += *m_str;

		std::cout << "parse word:" << *m_str << std::endl;
		m_str++;
		/* need to leave the last newline to be picked up as eol */
		if (*m_str == '\n') {
			m_str--;
			break;
		}
	}
	Token tok;
	is_number(word) ?  tok = { LITERAL, word } : tok = { ID, word };
	m_tokens.push_back(tok);
	return true;
}

/*
 * tokenize:
 * ---------
 * transforms the class's char * to a vector of tokens using the dfa and
 * parse_word
 */
vector<Token> Lexer::tokenize() {
	register_states();
	Token tok;
	while (*m_str) {
		if (*m_str == '\n') {
			m_tokens.push_back({ EOL, "" });
		} else if (m_dfa.recognize_string(m_str, tok)) {
			m_tokens.push_back(tok);
		} else {
			parse_word();
		}
		m_str++;
	}
	return m_tokens;
}
