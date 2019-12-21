#ifndef __PARSETREE_H__
#define __PARSETREE_H__

#include "tokens.h"
#include <vector>
using std::vector;

class ParseTreeNode {
public:
	ParseTreeNode(Token tok) : m_tok(tok) {}
	void set_children(vector<ParseTreeNode*> children);
private:
	vector<ParseTreeNode *> m_children;
	Token m_tok;
};

class ParseTree {
public:
	ParseTree(ParseTreeNode *root) : m_root(root) {}
	~ParseTree() {}
private:
	ParseTreeNode *m_root;
};

#endif /* __PARSETREE_H__ */
