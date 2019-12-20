#ifndef __TOKENS_H__
#define __TOKENS_H__
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
	IN
};

struct Token {
	Kind kind;
	std::string lexeme;
};
#endif /* __TOKENS_H__ */
