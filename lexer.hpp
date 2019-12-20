#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <regex>
#include "tokens.hpp"
#include "dfa.hpp"
#include "reader.hpp"

using std::vector;
using std::string;

class Lexer {
public:
	Lexer(const char *str) : m_str(str) { }
	~Lexer() {
		/* free the stringstream */
		m_ss.str(string());
		m_ss.clear();
	}
	vector<struct Token> parse_tokens();

private:
	DFA m_dfa;
	vector<struct Token> m_tokens;
	vector<struct state> m_states;
	std::stringstream m_ss;
	const char *m_str;

	void register_states();
	void register_types();
	void register_operators();
	void register_declarations();
	void register_eol();
	bool parse_word();
	bool is_number(string& s);
};

