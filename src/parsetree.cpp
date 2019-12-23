/*
 * parsetree.cpp
 * -------------
 * implements the ParseTreeNode class
 */

#include "parsetree.h"

typedef ParseTreeNode PNode;
typedef InteriorNode INode;
typedef LeafNode LNode;

/*
 ******************
 * interior nodes *
 ******************
 */
void InteriorNode::set_children(vector<shared_ptr<ParseTreeNode>> children) {
	m_children = children;
}

vector<shared_ptr<ParseTreeNode>> InteriorNode::get_children() {
	return m_children;
}

void InteriorNode::add_child(shared_ptr<ParseTreeNode> child) {
	m_children.push_back(child);
}

void InteriorNode::traverse() {
	std::cout << m_rule << std::endl;
	for (auto child : m_children) {
		child->traverse();
	}
}

void InteriorNode::traverse(int depth) {
	std::string d(depth * 2, '-');
	std::cout << d << ">" << m_rule << std::endl;
	depth++;
	for (auto child : m_children) {
		child->traverse(depth);
	}
}

/*
 * generate_vertex:
 * ----------------
 * writes a graphviz node to an open file stream
 * nodes are of the form: nodei[label="<f0>|...|<fn>"];
 */
void InteriorNode::generate_vertex(std::ofstream &fout) {
	fout << " node" << m_id << "[label = \"<f0> " << m_rule << "\"];" << endl;
}

/*
 * generate_edge:
 * ----------------
 * writes a graphviz edge to an open file stream
 * nodes are of the form: "nodei":fj->"nodek":fl;
 */
void InteriorNode::generate_edge(std::ofstream &fout) {
	for (size_t i = 0; i < m_children.size(); i++) {
		fout << "\"node" << m_id << "\":f" << 0 << "->\"node"
			<< m_children[i]->get_id() << "\":f0;" << std::endl;
	}
}

/*
 * show_tree:
 * ----------
 * writes a graphviz graph to a given filename
 */
void InteriorNode::show_tree(const char filename[]) {
	std::ofstream fout;
	fout.open(filename, std::ios::out | std::ios::trunc);
	fout << "digraph g{" << endl;
	fout << " node [shape = record,height=.1];" << endl;

	vector<PNode*> queue;
	size_t id = 0;

	/* perform a BFS of the tree to generate the nodes */
	queue.push_back(this);
	while (!queue.empty()) {
		PNode *P = queue.front();
		P->set_id(id);
		id++;
		queue.erase(queue.begin());
		if (auto I = dynamic_cast<INode*>(P)) {
			for (auto child : I->get_children()) {
				queue.push_back(&*child);
			}
		}
		P->generate_vertex(fout);
	}

	/* generate the edges with another BFS */
	queue.push_back(this);
	while (!queue.empty()) {
		PNode *P = queue.front();
		queue.erase(queue.begin());
		if (auto I = dynamic_cast<INode*>(P)) {
			for (auto child : I->get_children()) {
				queue.push_back(&*child);
			}
			I->generate_edge(fout);
		}
	}

	fout << "}" << std::endl;
}

void InteriorNode::set_id(size_t id) {
	m_id = id;
}
size_t InteriorNode::get_id() {
	return m_id;
}

/*
 **************
 * leaf nodes *
 **************
 */
void LeafNode::traverse() {
	std::cout << m_tok << std::endl;
}


void LeafNode::traverse(int depth) {
	std::string d(depth * 2, '-');
	std::cout << d << ">" << m_tok << std::endl;
}

void LeafNode::set_id(size_t id) {
	m_id = id;
}
size_t LeafNode::get_id() {
	return m_id;
}

void LeafNode::generate_vertex(std::ofstream &fout) {
	fout << " node" << m_id << "[label = \"<f0> " << m_tok << "\"];" << endl;
}

