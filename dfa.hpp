#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include "tokens.hpp"

using std::string;
using std::vector;

struct state {
	string str;
	Kind kind;
	string lexeme;
};

class DFA {
public:
	bool recognize_string(const char*& s, struct Token& tok);
	void register_state(struct state& st);
	void register_states(vector<struct state>& states);
private:
	vector<struct state> m_states;
};

