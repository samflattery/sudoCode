#ifndef __AST_H__
#define __AST_H__

#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include <vector>
#include "tokens.h"
#include "grammar.h"

using std::shared_ptr;
using std::endl;
using std::vector;

class AST {
public:
	virtual int execute_node()		= 0;
	virtual Token get_token()		= 0;
	virtual size_t get_id()			= 0;
	virtual void set_id(size_t id)	= 0;
	void generate_vertex(std::ofstream &fout);
};

class BinaryOperator : public AST {
public:
	BinaryOperator(Token t, shared_ptr<AST> l, shared_ptr<AST> r)
		: m_token(t), m_left(l), m_right(r) {}
	int execute_node();

	Token get_token()				{ return m_token; }
	virtual size_t get_id()			{ return m_id; }
	virtual void set_id(size_t id)	{ m_id = id; }

	void show_tree(const char filename[]);
private:
	void generate_edge(std::ofstream &fout);
	Token m_token;
	shared_ptr<AST> m_left;
	shared_ptr<AST> m_right;
	size_t m_id;
};

class Number : public AST {
public:
	Number(Token t) : m_token(t) {}
	int execute_node();
	Token get_token()				{ return m_token; }
	virtual size_t get_id()			{ return m_id; }
	virtual void set_id(size_t id)	{ m_id = id; }
private:
	Token m_token;
	size_t m_id;
};

#endif /* __AST_H__ */
