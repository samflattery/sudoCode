#include "grammar.h"

std::ostream& operator<<(std::ostream &os, Rule &r) {
	switch (r) {
		case ARITH_EXP:
			os << "ARITH_EXP";
			break;
		case EXP:
			os << "EXP";
			break;
		case TERM:
			os << "TERM";
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
		case DECLRTN:
			os << "DECLRTN";
			break;
		case DECLRTR:
			os << "DECLRTR";
			break;
		default:
			os << "not implemented";
			break;
	}
	return os;
}
