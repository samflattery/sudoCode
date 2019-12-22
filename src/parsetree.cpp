#include "parsetree.h"

void InteriorNode::set_children(vector<ParseTreeNode*> children) {
	m_children = children;
}

vector<ParseTreeNode*> InteriorNode::get_children() {
	return m_children;
}

void InteriorNode::add_child(ParseTreeNode *child) {
	m_children.push_back(child);
}

void LeafNode::traverse() {
	std::cout << m_tok << std::endl;
}

void InteriorNode::traverse() {
	std::cout << m_rule << std::endl;
	for (auto child : m_children) {
		child->traverse();
	}
}

void LeafNode::traverse(int depth) {
	std::string d(depth * 2, '-');
	std::cout << d << ">" << m_tok << std::endl;
}

void InteriorNode::traverse(int depth) {
	std::string d(depth * 2, '-');
	std::cout << d << ">" << m_rule << std::endl;
	depth++;
	for (auto child : m_children) {
		child->traverse(depth);
	}
}
