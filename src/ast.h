#ifndef __AST_H__
#define __AST_H__

#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include <vector>
#include "tokens.h"
#include "grammar.h"
#include "tree.h"
/* #include "symbols.h" */
#include "symboltable.h"

using std::shared_ptr;
using std::endl;
using std::vector;

class AST : public Tree {
public:
	virtual scVal execute_node()	= 0;
	virtual Token get_token()		= 0;
	virtual size_t get_id()			= 0;
	virtual void set_id(size_t id)	= 0;

	virtual shared_ptr<vector<shared_ptr<Tree>>> get_descendents() = 0;
	void generate_vertex(std::ofstream &fout);
	void generate_edge(std::ofstream &fout);
};

class MainFunction : public AST {
public:
	MainFunction(vector<shared_ptr<AST>> stmt_list) :
		m_stmts(stmt_list) {}
	virtual scVal execute_node();

	/* getters/setters */
	virtual Token get_token()		{ return m_ret; }
	virtual size_t get_id()			{ return m_id; }
	virtual void set_id(size_t id)	{ m_id = id; }
	virtual shared_ptr<vector<shared_ptr<Tree>>> get_descendents();
private:
	vector<shared_ptr<AST>> m_stmts;
	Token m_ret = {MAIN, ""};
	size_t m_id;
};

class BinaryOperator : public AST {
public:
	BinaryOperator(Token t, shared_ptr<AST> l, shared_ptr<AST> r)
		: m_token(t), m_left(l), m_right(r) {}
	scVal execute_node();

	/* getters/setters */
	Token get_token()				{ return m_token; }
	virtual size_t get_id()			{ return m_id; }
	virtual void set_id(size_t id)	{ m_id = id; }

	virtual shared_ptr<vector<shared_ptr<Tree>>> get_descendents();

private:
	Token m_token;
	shared_ptr<AST> m_left;
	shared_ptr<AST> m_right;
	size_t m_id;
};

class Number : public AST {
public:
	Number(Token t) : m_token(t) {}
	scVal execute_node();

	/* getters/setters */
	Token get_token()				{ return m_token; }
	virtual size_t get_id()			{ return m_id; }
	virtual void set_id(size_t id)	{ m_id = id; }

	virtual shared_ptr<vector<shared_ptr<Tree>>> get_descendents();
private:
	Token m_token;
	size_t m_id;
};

class Declarator : public AST {
public:
	Declarator(Token type, Token name) : m_type(type), m_name(name) {}
	scVal execute_node();

	/* getters/setters */
	Token get_type()				{ return m_type; }
	Token get_name()				{ return m_name; }
	virtual size_t get_id()			{ return m_id; }
	virtual void set_id(size_t id)	{ m_id = id; }
	virtual Token get_token()		{ return m_name; }

	virtual shared_ptr<vector<shared_ptr<Tree>>> get_descendents();
private:
	Token m_type;
	Token m_name;
	size_t m_id;
};

class Identifier : public AST {
public:
	Identifier(Token id) : m_identifier(id) {}
	scVal execute_node();

	/* getters/setters */
	virtual size_t get_id()			{ return m_id; }
	virtual void set_id(size_t id)	{ m_id = id; }
	virtual Token get_token()		{ return m_identifier; }

	virtual shared_ptr<vector<shared_ptr<Tree>>> get_descendents();
private:
	Token m_identifier;
	size_t m_id;
};

#endif /* __AST_H__ */
