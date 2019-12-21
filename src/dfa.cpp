#include "dfa.h"

/* register_string:
 * ----------------
 *  registers a new string associated with kind and lexeme to be a new accepting
 *  state of the DFA
 */
void DFA::register_state(struct state &st) {
	for (auto existing : m_states) {
		if (existing.str == st.str) {
			return;
		}
	}
	m_states.push_back(st);
}

/*
 * register_strings:
 * -----------------
 *  registers a set of strings, associated with kinds and lexemes
 */
void DFA::register_states(vector<struct state>& states) {
	for (auto st : states) {
		register_state(st);
	}
}

/* recognize_string:
 * -----------------
 *  returns true iff s has been registered as an accepting state
 *  if so, sets k and l to the corresponding kind and lexeme
 */
bool DFA::recognize_string(const char*& c, Token& tok) {
	const char *start = c;
	for (auto match : m_states) {
		unsigned int i;

		/* try and match the sequence of characters with the match string */
		for (i = 0; i < match.str.size(); i++) {
			if (*c != match.str[i]) {
				break;
			}
			c++;
		}

		/* failed to match */
		if (i != match.str.size()) {
			c = start;
			continue;
		}

		tok.kind = match.kind;
		tok.lexeme = match.lexeme;
		return true;
	}
	return false;
}
