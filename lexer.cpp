#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <regex>

using std::vector;
using std::string;

enum Kind {
	ASSIGN,
	LITERAL,
	ID,
	INT,
	STRING,
	CHAR,
	BOOL,
	BOOL_OP,
	COMP_OP,
	ARITH_OP,
	FUNC,
	MAIN,
	COND_STMT,
	WHILE,
	FOR,
	PRINT,
	LINE_END
};

struct Token {
	Kind kind;
	string lexeme;
};

std::ostream& operator<<(std::ostream &os, const Kind &k) {
	switch (k) {
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

std::ostream& operator<<(std::ostream &os, const Token &t) {
	return os << "Token: " << t.kind << "Lexeme: " << t.lexeme << std::endl;
}

class Lexer {
public:
	Lexer(std::string str) {
		m_ss.str(str);
	}
	~Lexer() {
		/* free the stringstream */
		m_ss.str(string());
		m_ss.clear();
	}
	vector<Token> parse_tokens();

private:
	vector<Token> m_tokens;
	std::stringstream m_ss;

	bool stream_has_string(string str);
	bool parse_expression();
	bool parse_assignment();
	bool parse_int();
	bool parse_arith_expression();
	bool parse_qualified_name();
	bool parse_literal();
	bool parse_name();
	bool parse_operator();
	bool parse_eol();
};

vector<Token> Lexer::parse_tokens() {
	while (m_ss.rdbuf()->in_avail()) {
		Lexer::parse_expression();
	}
	return m_tokens;
}

/* compare a string with the first n words of string stream */
bool Lexer::stream_has_string(string str) {

	std::cout << "stream_has_string: " << str << std::endl;

	std::stringstream s2;
	s2.str(str);

	string first;
	string second;

	while (s2 >> second && m_ss >> first) {
		std::cout << first << " " << second << std::endl;
		if (first != second) {
			return false;
		}
	}

	return true;
}

bool is_number(string& s) {
	string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}


bool Lexer::parse_expression() {
	/* parse an arithmetic expression or an assignment expression */
	/* declare that <qualified name> exists and instiantiate it to the value of <expression> */
	/* compute the value of <name> <operator> <name> and store it in <name> */
	string start;
	Token token;

	std::cout << "start: " << start << std::endl;
	/* base case: done parsing */
	if (!(m_ss >> start)) {
		return true;
	}

	std::cout << "start: " << start << std::endl;

	if (start == "declare") {
		Lexer::parse_assignment();
	} else if (start == "compute") {
		Lexer::parse_arith_expression();
	} else if (is_number(start)) {
		Token token;
		token.kind = LITERAL;
		token.lexeme = start;
		m_tokens.push_back(token);
	} else if (parse_eol()) {
		std::cout << "eol\n";
		/* parse_expression(); */
	} else {
		throw "Invalid token: ";
		return false;
	}

	/* done parsing */
	return true;
}

bool Lexer::parse_name() {
	string name;
	m_ss >> name;

	Token token;
	token.kind = ID;
	token.lexeme = name;

	m_tokens.push_back(token);
	std::cout << "parsed name: " << name << std::endl;

	return true;
}

bool Lexer::parse_eol() {
	std::cout << "parsing eol\n";
	if (stream_has_string("then end this line")) {
		return true;
	} else {
		std::cout << "false\n";
		return false;
	}
}

bool Lexer::parse_operator() {
	string first;
	string second;
	string op;

	m_ss >> first;
	if (first == "modulo") {
		op = "%";
	} else {
		m_ss >> second;
		first += " " + second;
		std::cout << first << std::endl;
		if (first == "added to") {
			op = "+";
		} else if (first == "subtracted from") {
			op = "-";
		} else if (first == "multiplied by") {
			op = "*";
		} else if (first == "divided by") {
			op = "/";
		} else {
			throw "Invalid";
		}
	}

	Token token;
	token.kind = ARITH_OP;
	token.lexeme = op;
	m_tokens.push_back(token);

	std::cout << "parsed operator: " << op << std::endl;

	return false;
}

bool Lexer::parse_arith_expression() {
	if (!stream_has_string("the value of")) {
		throw "Invalid";
		return false;
	}

	std::cout << "has the value of\n";

	parse_name();
	parse_operator();
	parse_name();

	std::cout << "has three things\n";

	if (!stream_has_string("and store it in")) {
		throw "Invalid";
		return false;
	}

	Token token;
	token.kind = ASSIGN;
	token.lexeme = "";
	m_tokens.push_back(token);

	parse_name();

	return true;
}


bool Lexer::parse_assignment() {

	if (!stream_has_string("that")) {
		throw "Invalid";
	}

	if (!Lexer::parse_qualified_name()) {
		return false;
	}

	Token token;
	token.kind = ASSIGN;
	token.lexeme = "";
	m_tokens.push_back(token);

	if (!stream_has_string("exists and instantiate it to the value of")) {
		throw "Invalid";
	}

	if (!Lexer::parse_expression()) {
		return false;
	}

	return true;
}

bool Lexer::parse_qualified_name() {
	string first_char;
	m_ss >> first_char;
	if (first_char != "a") {
		throw "Invalid token: expected 'a'";
		return false;
	}

	string name;
	m_ss >> name;
	Kind kind;
	if (name == "whole") {
		if (stream_has_string("number")) {
			kind = INT;
		} else {
			throw "Invalid";
		}
	} else if (name == "sequence") {
		if (stream_has_string("of characters")) {
			kind = STRING;
		} else {
			throw "Invalid";
		}
	} else if (name == "character") {
		kind = CHAR;
	} else {
		throw "Invalid";
	}

	/* check if name is not reserved and then add to token struct */
	/* check if type is valid */
	Token token;
	token.kind = kind;
	token.lexeme = "";
	m_tokens.push_back(token);

	if (!stream_has_string("called")) {
		return false;
	}

	if (!parse_name()) {
		return false;
	}

	return true;
}

int main() {
	string test = "declare that a whole number called x exists and instantiate it to the value of 5 and then end this line\n"
					"declare that a whole number called y exists and instantiate it to the value of 6 and then end this line\n"
					"compute the value of x added to y and store it in x and then end this line";
	Lexer lexer(test);
	vector<Token> tokens = lexer.parse_tokens();

	for (vector<Token>::const_iterator i = tokens.begin(); i < tokens.end(); i++) {
		std::cout << *i << std::endl;

	}

	return 0;
}
