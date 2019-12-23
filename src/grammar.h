/*
 * grammar.h
 * ---------
 * defines the grammar rules that are used by the parser when generating the
 * parse tree
 */

#ifndef __GRAMMAR_H__
#define __GRAMMAR_H__

#include <ostream>

enum Rule {
	ASSIGNR,
	CONST,
	DECLRTR,
	DECLRTN,
	UNARY_EXP,
	ASSIGN_EXP,
	ARITH_EXP,
	PRIM_EXP,
	EXP,
	TERM,
	FACTOR,
	STMT,
	CMPD_STMT,
	EXP_STMT,
	IF_STMT,
	ITER_STMT,
	JMP_STMT,
	DECLR_LST,
	MAIN_FUN
};

std::ostream& operator<<(std::ostream &os, Rule &r);

#endif /* __GRAMMAR_H__ */
