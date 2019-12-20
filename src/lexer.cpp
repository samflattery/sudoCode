#include "lexer.hpp"

/* print kinds */
std::ostream& operator<<(std::ostream &os, const Kind &k) {
	switch (k) {
		case IN:
			os << "IN";
			break;
		case CALLD:
			os << "CALLED";
			break;
		case EOL:
			os << "EOL";
			break;
		case DECLR:
			os << "DECLARATION";
			break;
		case ASSIGN:
			os << "ASSIGN";
			break;
		case ID:
			os << "ID";
			break;
		case INT:
			os << "INT";
			break;
		case ARITH_OP:
			os << "ARITH_OP";
			break;
		case LITERAL:
			os << "LITERAL";
			break;
		default:
			os << "not implemented";
			break;
	}
	return os << std::endl;
}

/* print tokens */
std::ostream& operator<<(std::ostream &os, const Token &t) {
	return os << "Token: " << t.kind << "Lexeme: " << t.lexeme << std::endl;
}

bool is_number(string& s) {
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
										{ "modulu", ARITH_OP, "%" }
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
	std::string x;
	while (*m_str != ' ') {
		x += *m_str;
		m_str++;
	}
	struct Token tok = { ID, x };
	m_tokens.push_back(tok);
	return true;
}

vector<Token> Lexer::parse_tokens() {
	register_states();
	struct Token tok;
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

int main() {
	Reader r("docs/test.txt");
	size_t length;
	const char *c = r.read_file(length);
	if (c == NULL) {
		std::cout << "Failed to read file" << std::endl;
		return 1;
	}
	std::cout << c << std::endl;

	Lexer lexer(c);
	vector<Token> tokens = lexer.parse_tokens();

	for (vector<Token>::const_iterator i = tokens.begin(); i < tokens.end(); i++) {
		std::cout << *i << std::endl;

	}

	return 0;
}
