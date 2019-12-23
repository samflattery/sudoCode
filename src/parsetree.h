/*
 * parsetree.h
 * -----------
 * defines the nodes of a parse tree, which are either leaves or interior nodes
 * and inherit from ParseTreeNode
 */

#ifndef __PARSETREE_H__
#define __PARSETREE_H__

#include "grammar.h"
#include "tokens.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>

using std::endl;
using std::vector;
using std::shared_ptr;

class ParseTreeNode {
public:
	virtual void traverse() = 0;
	virtual void traverse(int) = 0;
	virtual ~ParseTreeNode() {}
	virtual void set_id(size_t id) = 0;
	virtual size_t get_id() = 0;
	virtual void generate_vertex(std::ofstream &fout) = 0;
private:
};

class InteriorNode : public ParseTreeNode {
public:
	InteriorNode(Rule rule) : m_rule(rule), m_id(0) {}
	~InteriorNode() {}
	void set_children(vector<shared_ptr<ParseTreeNode>> children);
	vector<shared_ptr<ParseTreeNode>> get_children();
	void add_child(shared_ptr<ParseTreeNode> child);
	Rule m_rule;
	void traverse();
	void traverse(int depth);
	void show_tree(const char filename[]);
	virtual void set_id(size_t id);
	virtual size_t get_id();
	virtual void generate_vertex(std::ofstream &fout);
private:
	vector<shared_ptr<ParseTreeNode>> m_children;
	size_t m_id;
	void generate_edge(std::ofstream &fout);
};

class LeafNode : public ParseTreeNode {
public:
	LeafNode(Token tok) : m_tok(tok), m_id(0) {}
	~LeafNode() {}
	Token m_tok;
	void traverse();
	void traverse(int depth);
	virtual void set_id(size_t id);
	virtual size_t get_id();
	virtual void generate_vertex(std::ofstream &fout);
private:
	size_t m_id;
};

#endif /* __PARSETREE_H__ */
