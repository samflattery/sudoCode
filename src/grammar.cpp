#include "grammar.h"

std::ostream& operator<<(std::ostream &os, Rule &r) {
	switch (r) {
		case EXP:
			os << "EXP";
			break;
		case ADD_EXP:
			os << "ADD_EXP";
			break;
		case MULT_EXP:
			os << "MULT_EXP";
			break;
		case FACTOR:
			os << "FACTOR";
			break;
		case STMT:
			os << "STMT";
			break;
		case PRIM_EXP:
			os << "PRIM_EXP";
			break;
		case MAIN_FUN:
			os << "MAIN_FUN";
			break;
		case EXP_STMT:
			os << "EXP_STMT";
			break;
		case ASSIGN_EXP:
			os << "ASSIGN_EXP";
			break;
		case DECLRTN:
			os << "DECLRTN";
			break;
		case DECLRTR:
			os << "DECLRTR";
			break;
		case ASSIGNR:
			os << "ASSIGNR";
			break;
		default:
			os << "not implemented";
			break;
	}
	return os;
}
