/*
 * tokens.h
 * --------
 * defines the tokens that the lexer generates
 */

#ifndef __TOKENS_H__
#define __TOKENS_H__

#include <string>
#include <ostream>

enum Kind {
	CALLD,
	DECLR,
	ASSIGN,
	LITERAL,
	EOL,
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
	IN,
	LPAREN,
	RPAREN
};

struct Token {
	Kind kind;
	std::string lexeme;
};

std::ostream& operator<<(std::ostream &os, const Kind &k);
std::ostream& operator<<(std::ostream &os, const Token &t);

#endif /* __TOKENS_H__ */
