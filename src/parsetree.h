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
#include "tree.h"

using std::endl;
using std::vector;
using std::shared_ptr;
using std::make_shared;

class ParseTreeNode : public Tree {
public:
	virtual void set_id(size_t id)		= 0;
	virtual size_t get_id()				= 0;

	virtual void generate_edge(std::ofstream &fout) = 0;
	virtual void generate_vertex(std::ofstream &fout) = 0;
	virtual shared_ptr<vector<shared_ptr<Tree>>> get_descendents() = 0;

	virtual shared_ptr<AST> to_ast()	= 0;
	virtual bool get_token(Token *tok)	= 0;
private:
};

class InteriorNode : public ParseTreeNode {
public:
	InteriorNode(Rule rule) : m_rule(rule), m_id(0) {}

	/* manipulating children */
	void set_children(vector<shared_ptr<ParseTreeNode>> children);
	vector<shared_ptr<ParseTreeNode>> get_children();
	void add_child(shared_ptr<ParseTreeNode> child);
	shared_ptr<ParseTreeNode> pop_child();

	/* tree interface */
	virtual shared_ptr<vector<shared_ptr<Tree>>> get_descendents();
	virtual void generate_edge(std::ofstream &fout);
	virtual void generate_vertex(std::ofstream &fout);

	/* getters/setters */
	/* hack to avoid casting PNodes to Leaf or Interior Nodes */
	bool get_token(Token *tok)		{ *tok = m_tok; return false; }
	virtual void set_id(size_t id)	{ m_id = id; }
	virtual size_t get_id()			{ return m_id; }

	/* generate AST */
	shared_ptr<AST> to_ast();

	/* traversing/generating graph */
	void show_tree(const char filename[]);
private:
	vector<shared_ptr<ParseTreeNode>> m_children;
	Token m_tok = { ID, "x" };
	Rule m_rule;
	size_t m_id;
};

class LeafNode : public ParseTreeNode {
public:
	LeafNode(Token tok) : m_tok(tok), m_id(0) {}

	/* getters/setters */
	bool get_token(Token *tok)		{ *tok = m_tok; return true; }
	virtual void set_id(size_t id)	{ m_id = id; }
	virtual size_t get_id()			{ return m_id; }

	/* generate AST */
	shared_ptr<AST> to_ast();

	/* tree interface */
	virtual void generate_vertex(std::ofstream &fout);
	virtual void generate_edge(std::ofstream &fout);
	virtual shared_ptr<vector<shared_ptr<Tree>>> get_descendents();
private:
	Token m_tok;
	size_t m_id;
};

typedef ParseTreeNode PNode;
typedef InteriorNode INode;
typedef LeafNode LNode;


#endif /* __PARSETREE_H__ */
