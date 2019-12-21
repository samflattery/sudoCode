#include "tokens.h"

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
	return os;
}

std::ostream& operator<<(std::ostream &os, const Token &t) {
	return os << "Token: " << t.kind << "\tLexeme: " << t.lexeme << std::endl;
}
