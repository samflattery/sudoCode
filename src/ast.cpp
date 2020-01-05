#include "ast.h"

SymbolTable symbolTable;

scVal MainFunction::execute_node() {
	for (vector<shared_ptr<AST>>::size_type i = 0; i != m_stmts.size(); ++i) {
		auto node = m_stmts[i];
		if (i == m_stmts.size() - 1) {
			return node->execute_node();
		}
		node->execute_node();
	}
	return 0;
}

shared_ptr<vector<shared_ptr<Tree>>> MainFunction::get_descendents() {
	shared_ptr<vector<shared_ptr<Tree>>> ptr =
		std::make_shared<vector<shared_ptr<Tree>>>();
	for (auto st : m_stmts) {
		ptr->push_back(st);
	}
	return ptr;
}

shared_ptr<vector<shared_ptr<Tree>>> BinaryOperator::get_descendents() {
	shared_ptr<vector<shared_ptr<Tree>>> ptr =
		std::make_shared<vector<shared_ptr<Tree>>>();
	ptr->push_back(m_left);
	ptr->push_back(m_right);
	return ptr;
}

shared_ptr<vector<shared_ptr<Tree>>> Number::get_descendents() {
	shared_ptr<vector<shared_ptr<Tree>>> ptr =
		std::make_shared<vector<shared_ptr<Tree>>>();
	return ptr;
}

shared_ptr<vector<shared_ptr<Tree>>> Identifier::get_descendents() {
	shared_ptr<vector<shared_ptr<Tree>>> ptr =
		std::make_shared<vector<shared_ptr<Tree>>>();
	return ptr;
}

shared_ptr<vector<shared_ptr<Tree>>> Declarator::get_descendents() {
	shared_ptr<vector<shared_ptr<Tree>>> ptr =
		std::make_shared<vector<shared_ptr<Tree>>>();
	return ptr;
}

scVal BinaryOperator::execute_node() {
	scVal l_res = m_left->execute_node();
	scVal r_res = m_right->execute_node();
	switch (m_token.kind) {
		case MUL_OP:
			{
			int l = std::get<int>(l_res);
			int r = std::get<int>(r_res);
			if (m_token.lexeme == "*") {
				return l * r;
			} else if (m_token.lexeme == "%"){
				return l % r;
			} else {
				return l / r;
			}
			}
		case ADD_OP:
			{
			int l = std::get<int>(l_res);
			int r = std::get<int>(r_res);
			if (m_token.lexeme == "+") {
				return l + r;
			} else {
				return l - r;
			}
			}
		case DECLR:
			{
			string info = std::get<string>(l_res);
			char type = info[0];
			string id = info.substr(1, info.size());
			try {
				switch (type) {
					case '0':
						if (!std::holds_alternative<int>(r_res))
							throw "invalid";
						break;
					case '1':
						if (!std::holds_alternative<char>(r_res))
							throw "invalid";
						break;
						break;
					case '2':
						if (!std::holds_alternative<string>(r_res))
							throw "invalid";
						break;
					default:
						exit(0); /* should be impossible as would fail at
									the get above */
				}
			} catch (const std::exception&) {
				std::cerr << "type mismatch\n";
				exit(0);
			}
			symbolTable.add_symbol(id, r_res);
			std::cout << std::get<int>(r_res) << std::endl;
			return r_res;
			}
		case ASSIGN:
			return -1;
		default:
			return -1;
	}
}

scVal Number::execute_node() {
	int val = std::stoi(m_token.lexeme);
	return val;
}

scVal Identifier::execute_node() {
	pair<scVal, bool> ret = symbolTable.lookup_symbol(m_identifier.lexeme);
	if (ret.second == false) {
		throw "undefined variable\n";
	}
	return ret.first;
}

scVal Declarator::execute_node() {
	/* return a string, the first character of which represents the type info
	 * and the remaining characters are the name of the variable */
	std::string ret;
	switch (m_type.kind) {
		case INT:
			ret += '0';
			break;
		case CHAR:
			ret += '1';
			break;
		case STRING:
			ret += '2';
			break;
		default:
			return -1;
	}
	ret += m_name.lexeme;
	return ret;
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
		fout << " \"node" << get_id() << "\":f0->\"node" << x->get_id()
			<< "\":f0;" << std::endl;
	}
}
