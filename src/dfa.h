/*
 * dfa.h
 * -------
 * defines a DFA class that recognizes registered strings to help the lexer
 * recognize tokens
 */

#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include "tokens.h"

#ifndef __DFA_H__
#define __DFA_H__

using std::string;
using std::vector;

struct state {
	string str;
	Kind kind;
	string lexeme;
};

class DFA {
public:
	bool recognize_string(const char*& s, Token& tok);
	void register_state(struct state& st);
	void register_states(vector<struct state>& states);
private:
	vector<struct state> m_states;
};

#endif /* __DFA_H__ */
