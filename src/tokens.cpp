#include "tokens.h"

std::ostream& operator<<(std::ostream &os, const Kind &k) {
	switch (k) {
		case EXIST:
			os << "EXIST";
			break;
		case VALUE:
			os << "VALUE";
			break;
		case CALLD:
			os << "CALLED";
			break;
		case EOL:
			os << "EOL";
			break;
		case END:
			os << "EOL";
			break;
		case DECLR:
			os << "DECLR";
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
		case STRING:
			os << "STRING";
			break;
		case CHAR:
			os << "CHAR";
			break;
		case ADD_OP:
			os << "ADD_OP";
			break;
		case MUL_OP:
			os << "MUL_OP";
			break;
		case LITERAL:
			os << "LITERAL";
			break;
		case LPAREN:
			os << "LPAREN";
			break;
		case RPAREN:
			os << "RPAREN";
			break;
		case MAIN:
			os << "MAIN";
			break;
		default:
			os << "not implemented";
			break;
	}
	return os;
}

std::ostream& operator<<(std::ostream &os, const Token &t) {
	return os << "Token: " << t.kind << "\tLexeme: " << t.lexeme;
}
