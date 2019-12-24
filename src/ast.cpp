#include "ast.h"

int BinaryOperator::execute_node() {
	int l_res = m_left->execute_node();
	int r_res = m_right->execute_node();
	switch (m_token.kind) {
		case MUL_OP:
			if (m_token.lexeme == "*") {
				return l_res * r_res;
			} else if (m_token.lexeme == "%"){
				return l_res % r_res;
			} else {
				return l_res / r_res;
			}
		case ADD_OP:
			if (m_token.lexeme == "+") {
				return l_res + r_res;
			} else {
				return l_res - r_res;
			}
		default:
			return -1;
	}
}

int Number::execute_node() {
	int val = std::stoi(m_token.lexeme);
	return val;
}

void AST::generate_vertex(std::ofstream &fout) {
	fout << " node" << get_id() << "[label = \"<f0> " << get_token() << "\"];" << endl;
}

/*
 * generate_edge:
 * --------------
 * writes a graphviz edge to an open file stream
 * nodes are of the form: "nodei":fj->"nodek":fl;
 */
void BinaryOperator::generate_edge(std::ofstream &fout) {
	fout << "\"node" << m_id << "\":f0->\"node" << m_left->get_id() << "\":f0;" << std::endl;
	fout << "\"node" << m_id << "\":f0->\"node" << m_right->get_id() << "\":f0;" << std::endl;
}

/*
 * show_tree:
 * ----------
 * writes a graphviz graph to a given filename
 */
void BinaryOperator::show_tree(const char filename[]) {
	std::ofstream fout;
	fout.open(filename, std::ios::out | std::ios::trunc);
	fout << "digraph g{" << endl;
	fout << " node [shape = record,height=.1];" << endl;

	vector<AST*> queue;
	size_t id = 0;

	/* perform a BFS of the tree to generate the nodes */
	queue.push_back(this);
	while (!queue.empty()) {
		AST *P = queue.front();
		P->set_id(id);
		id++;
		queue.erase(queue.begin());
		if (auto I = dynamic_cast<BinaryOperator*>(P)) {
			queue.push_back(&*I->m_left);
			queue.push_back(&*I->m_right);
		}
		P->generate_vertex(fout);
	}

	/* generate the edges with another BFS */
	queue.push_back(this);
	while (!queue.empty()) {
		AST *P = queue.front();
		queue.erase(queue.begin());
		if (auto I = dynamic_cast<BinaryOperator*>(P)) {
			queue.push_back(&*I->m_left);
			queue.push_back(&*I->m_right);
			I->generate_edge(fout);
		}
	}

	fout << "}" << std::endl;
}

