/*
 * lexer.cpp
 * ---------
 * defines a lexer that transforms a sequence of characters read from a file
 * into a vector of tokens
 */

#ifndef __LEXER_H__
#define __LEXER_H__

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <regex>
#include "tokens.h"
#include "dfa.h"
#include "reader.h"

using std::vector;
using std::string;

class Lexer {
public:
	Lexer(const char *str) : m_str(str) { }
	vector<Token> tokenize();

private:
	DFA m_dfa;
	vector<Token> m_tokens;
	vector<struct state> m_states;
	const char *m_str;

	void register_states();
	void register_types();
	void register_operators();
	void register_declarations();
	void register_eol();
	bool parse_word();
	bool is_number(string& s);
};

#endif /*__LEXER_H__ */
