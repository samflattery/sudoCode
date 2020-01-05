/*
 * parsetree.cpp
 * -------------
 * implements the ParseTreeNode class
 */

#include "parsetree.h"

/*
 **********
 * to_ast *
 **********
 */
shared_ptr<AST> InteriorNode::to_ast() {
	vector<shared_ptr<AST>> children;
	switch (m_rule) {
		case MAIN_FUN:
			{
			vector<shared_ptr<AST>> stmts;
			for (auto child : m_children) {
				stmts.push_back(child->to_ast());
			}
			shared_ptr<AST> func = make_shared<MainFunction>(stmts);
			return func;
			}
		case STMT:
		case EXP_STMT:
		case EXP:
		case PRIM_EXP:
			/* collapse rules that only have one operand */
			return m_children[0]->to_ast();
		case ADD_EXP:
		case MULT_EXP:
			{
			shared_ptr<PNode> l_child = pop_child();

			shared_ptr<AST> l_node = l_child->to_ast();
			if (m_children.empty()) {
				return l_node;
			}
			Token tok;
			pop_child()->get_token(&tok);
			shared_ptr<AST> r_node = this->to_ast();
			shared_ptr<AST> op = make_shared<BinaryOperator>(tok, l_node, r_node);
			return op;
			}
		case FACTOR:
			if (m_children.size() == 1) {
				/* primary expression case */
				return m_children[0]->to_ast();
			}
			/* lparen expr rparen case */
			return m_children[1]->to_ast();
		case DECLRTN:
			{
			pop_child();
			shared_ptr<AST> declrtr = pop_child()->to_ast();
			pop_child();
			shared_ptr<AST> expr = pop_child()->to_ast();
			Token tok = {DECLR, ""};
			shared_ptr<AST> op = make_shared<BinaryOperator>(tok, declrtr, expr);
			return op;
			}
		case DECLRTR:
			{
			Token type_tok;
			Token id_tok;
			pop_child()->get_token(&type_tok);
			pop_child();
			pop_child()->get_token(&id_tok);
			shared_ptr<AST> op = make_shared<Declarator>(type_tok, id_tok);
			return op;
			}
		case ASSIGN_EXP:
			{
			shared_ptr<AST> l_child = pop_child()->to_ast();
			shared_ptr<AST> r_child = pop_child()->to_ast();
			if (l_child == nullptr) {
				/* it's a declaration */
				return r_child;
			}
			return nullptr;
			}
		case ASSIGNR:
			{
			pop_child();
			shared_ptr<PNode> id = pop_child();
			Token id_tok;
			id->get_token(&id_tok);
			if (id_tok.lexeme == "it") {
				/* declaration */
				return nullptr;
			}
			return nullptr;
			}
		default:
			return nullptr;
	}
}

shared_ptr<AST> LeafNode::to_ast() {
	shared_ptr<AST> ast;
	switch (m_tok.kind) {
		case LITERAL:
			return make_shared<Number>(m_tok);
		case ID:
			return make_shared<Identifier>(m_tok);
		default:
			return nullptr;
	}
}

/*
 ******************
 * util functions *
 ******************
 */
shared_ptr<PNode> InteriorNode::pop_child() {
	shared_ptr<PNode> first = m_children[0];
	m_children.erase(m_children.begin());
	return first;
}

void InteriorNode::set_children(vector<shared_ptr<ParseTreeNode>> children) {
	m_children = children;
}

vector<shared_ptr<ParseTreeNode>> InteriorNode::get_children() {
	return m_children;
}

void InteriorNode::add_child(shared_ptr<ParseTreeNode> child) {
	m_children.push_back(child);
}


/*
 ************************
 * Implement Tree Class *
 ************************
 */
void LeafNode::generate_vertex(std::ofstream &fout) {
	fout << " node" << m_id << "[label = \"<f0> " << m_tok << "\"];" << endl;
}

void LeafNode::generate_edge(std::ofstream &fout) {
	/* do nothing */
	fout << "";
}

shared_ptr<vector<shared_ptr<Tree>>> LeafNode::get_descendents() {
	shared_ptr<vector<shared_ptr<Tree>>> ptr =
		make_shared<vector<shared_ptr<Tree>>>();
	return ptr;
}

shared_ptr<vector<shared_ptr<Tree>>> InteriorNode::get_descendents() {
	shared_ptr<vector<shared_ptr<Tree>>> ptr =
		make_shared<vector<shared_ptr<Tree>>>();
	for (auto x : m_children) {
		ptr->push_back(x);
	}
	return ptr;
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
 * --------------
 * writes a graphviz edge to an open file stream
 * nodes are of the form: "nodei":fj->"nodek":fl;
 */
void InteriorNode::generate_edge(std::ofstream &fout) {
	for (size_t i = 0; i < m_children.size(); i++) {
		fout << "\"node" << m_id << "\":f" << 0 << "->\"node"
			<< m_children[i]->get_id() << "\":f0;" << std::endl;
	}
}
