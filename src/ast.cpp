#include "ast.h"

int MainFunction::execute_node() {
	for (auto node : m_stmts) {
		node->execute_node();
	}
	return 0;
}

shared_ptr<vector<shared_ptr<Tree>>> MainFunction::get_descendents() {
	shared_ptr<vector<shared_ptr<Tree>>> ptr = std::make_shared<vector<shared_ptr<Tree>>>();
	/* *ptr = m_stmts; */
	for (auto st : m_stmts) {
		ptr->push_back(st);
		std::cout << st->get_token() << std::endl;;
	}
	std::cout << "getting main" << std::endl;
	return ptr;
}

shared_ptr<vector<shared_ptr<Tree>>> BinaryOperator::get_descendents() {
	shared_ptr<vector<shared_ptr<Tree>>> ptr = std::make_shared<vector<shared_ptr<Tree>>>();
	std::cout << "getting binop" << std::endl;
	ptr->push_back(m_left);
	ptr->push_back(m_right);
	return ptr;
}

shared_ptr<vector<shared_ptr<Tree>>> Number::get_descendents() {
	shared_ptr<vector<shared_ptr<Tree>>> ptr = std::make_shared<vector<shared_ptr<Tree>>>();
	std::cout << "getting num" << std::endl;
	return ptr;
}

shared_ptr<vector<shared_ptr<Tree>>> Declarator::get_descendents() {
	shared_ptr<vector<shared_ptr<Tree>>> ptr = std::make_shared<vector<shared_ptr<Tree>>>();
	std::cout << "getting declr" << std::endl;
	return ptr;
}

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
		case ASSIGN:
			return -1;
		default:
			return -1;
	}
}

int Number::execute_node() {
	int val = std::stoi(m_token.lexeme);
	return val;
}

/* generate_vertex:
 * ----------------
 * writes a graphviz vertex to an open file stream
 * vertices are of the form: nodei [label = "<f0> <token>"];
 */
void AST::generate_vertex(std::ofstream &fout) {
	fout << " node" << get_id() << "[label = \"<f0> " << get_token() << "\"];"
		<< endl;
}

/*
 * generate_edge:
 * --------------
 * writes a graphviz edge to an open file stream
 * edges are of the form: "nodei":fj->"nodek":fl;
 */
void AST::generate_edge(std::ofstream &fout) {
	shared_ptr<vector<shared_ptr<Tree>>> desc = get_descendents();
	for (auto x : *desc) {
		fout << "\"node" << get_id() << "\":f0->\"node" << x->get_id()
			<< "\":f0;" << std::endl;
	}
}

/*
 * show_tree:
 * ----------
 * writes a graphviz graph to a given filename
 */
/* void MainFunction::show_tree(const char filename[]) { */
/* 	std::ofstream fout; */
/* 	fout.open(filename, std::ios::out | std::ios::trunc); */
/* 	fout << "digraph g{" << endl; */
/* 	fout << " node [shape = record,height=.1];" << endl; */

/* 	/1* vector<AST*> queue; *1/ */
/* 	vector<shared_ptr<Tree>> queue; */
/* 	size_t id = 0; */

/* 	/1* perform a BFS of the tree to generate the nodes *1/ */
/* 	queue.push_back(nullptr); */
/* 	while (!queue.empty()) { */
/* 		shared_ptr<Tree> P = queue.front(); */
/* 		queue.erase(queue.begin()); */
/* 		if (P == nullptr) { */
/* 			set_id(id); */
/* 			id++; */
/* 			shared_ptr<vector<shared_ptr<Tree>>> desc = get_descendents(); */
/* 			for (auto x : *desc) { */
/* 				queue.push_back(x); */
/* 			} */
/* 			generate_vertex(fout); */
/* 			continue; */
/* 		} */
/* 		P->set_id(id); */
/* 		id++; */
/* 		shared_ptr<vector<shared_ptr<Tree>>> desc = P->get_descendents(); */
/* 		for (auto x : *desc) { */
/* 			std::cout << x->get_id(); */
/* 			queue.push_back(x); */
/* 		} */
/* 		P->generate_vertex(fout); */
/* 	} */

/* 	/1* generate the edges with another BFS *1/ */
/* 	queue.push_back(nullptr); */
/* 	while (!queue.empty()) { */
/* 		shared_ptr<Tree> P = queue.front(); */
/* 		queue.erase(queue.begin()); */
/* 		if (P == nullptr) { */
/* 			shared_ptr<vector<shared_ptr<Tree>>> desc = get_descendents(); */
/* 			for (auto x : *desc) { */
/* 				queue.push_back(x); */
/* 			} */
/* 			generate_edge(fout); */
/* 			continue; */
/* 		} */
/* 		shared_ptr<vector<shared_ptr<Tree>>> desc = P->get_descendents(); */
/* 		for (auto x : *desc) { */
/* 			queue.push_back(x); */
/* 		} */
/* 		P->generate_edge(fout); */
/* 	} */

/* 	fout << "}" << std::endl; */
/* } */

