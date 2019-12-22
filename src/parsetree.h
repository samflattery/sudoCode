#ifndef __PARSETREE_H__
#define __PARSETREE_H__

#include "grammar.h"
#include "tokens.h"
#include <vector>
#include <iostream>
using std::vector;

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
	void set_children(vector<ParseTreeNode*> children);
	vector<ParseTreeNode*> get_children();
	void add_child(ParseTreeNode *child);
	Rule m_rule;
	void traverse();
	void traverse(int depth);
private:
	vector<ParseTreeNode *> m_children;
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
