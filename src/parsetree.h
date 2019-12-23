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
#include <memory>
using std::vector;
using std::shared_ptr;

class ParseTreeNode {
public:
	virtual void traverse() = 0;
	virtual void traverse(int) = 0;
	virtual ~ParseTreeNode() {}
};

class InteriorNode : public ParseTreeNode {
public:
	InteriorNode(Rule rule) : m_rule(rule) {}
	~InteriorNode() {}
	void set_children(vector<shared_ptr<ParseTreeNode>> children);
	vector<shared_ptr<ParseTreeNode>> get_children();
	void add_child(shared_ptr<ParseTreeNode> child);
	Rule m_rule;
	void traverse();
	void traverse(int depth);
private:
	vector<shared_ptr<ParseTreeNode>> m_children;
};

class LeafNode : public ParseTreeNode {
public:
	LeafNode(Token tok) : m_tok(tok) {}
	~LeafNode() {}
	Token m_tok;
	void traverse();
	void traverse(int depth);
private:

};

#endif /* __PARSETREE_H__ */
