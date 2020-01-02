/*
 * parsetree.h
 * -----------
 * defines the nodes of a parse tree, which are either leaves or interior nodes
 * and inherit from ParseTreeNode
 */

#ifndef __PARSETREE_H__
#define __PARSETREE_H__

#include <vector>
#include <iostream>
#include <fstream>
#include <memory>
#include "grammar.h"
#include "tokens.h"
#include "ast.h"

using std::endl;
using std::vector;
using std::shared_ptr;
using std::make_shared;

class ParseTreeNode {
public:
	virtual void traverse() = 0;
	virtual void traverse(int) = 0;
	virtual ~ParseTreeNode() {}
	virtual void set_id(size_t id) = 0;
	virtual size_t get_id() = 0;
	virtual shared_ptr<AST> to_ast() = 0;
	virtual void generate_vertex(std::ofstream &fout) = 0;
	virtual bool get_token(Token *tok) = 0;
private:
};

class InteriorNode : public ParseTreeNode {
public:
	InteriorNode(Rule rule) : m_rule(rule), m_id(0) {}
	~InteriorNode() {}

	/* util functions */
	bool get_token(Token *tok) { return false; }
	void set_children(vector<shared_ptr<ParseTreeNode>> children);
	vector<shared_ptr<ParseTreeNode>> get_children();
	void add_child(shared_ptr<ParseTreeNode> child);
	shared_ptr<ParseTreeNode> pop_child();

	Rule m_rule;

	/* generate AST */
	shared_ptr<AST> to_ast();

	/* traversing/generating graph */
	void traverse();
	void traverse(int depth);
	void show_tree(const char filename[]);
	virtual void set_id(size_t id)	{ m_id = id; }
	virtual size_t get_id()			{ return m_id; }
	virtual void generate_vertex(std::ofstream &fout);
private:
	vector<shared_ptr<ParseTreeNode>> m_children;
	/* used to generate the graph */
	size_t m_id;
	void generate_edge(std::ofstream &fout);
};

class LeafNode : public ParseTreeNode {
public:
	LeafNode(Token tok) : m_tok(tok), m_id(0) {}
	~LeafNode() {}
	Token m_tok;
	bool get_token(Token *tok) { *tok = m_tok; return true; }

	/* generate AST */
	shared_ptr<AST> to_ast();

	/* traversing/generating graph */
	void traverse();
	void traverse(int depth);
	virtual void set_id(size_t id)	{ m_id = id; }
	virtual size_t get_id()			{ return m_id; }
	virtual void generate_vertex(std::ofstream &fout);
private:
	size_t m_id;
};

#endif /* __PARSETREE_H__ */
