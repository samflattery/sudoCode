#include "symboltable.h"

void SymbolTable::add_symbol(string symb, scVal v) {
	std::cout << "added " << symb << " with val: " << std::get<int>(v) << std::endl;
	m_map[symb] = v;
}

pair<scVal, bool> SymbolTable::lookup_symbol(string symb) {
	pair<scVal, bool> p;
	if (m_map.find(symb) != m_map.end()) {
		p.first = m_map[symb];
		p.second = true;
		return p;
	}
	scVal v = 0;
	p.first = v;
	p.second = false;
	return p;
}

shared_ptr<SymbolTable> SymbolTable::create_scope() {
	shared_ptr<SymbolTable> ST = std::make_shared<SymbolTable>();
	ST->set_enclosing_scope(shared_from_this());
	return ST;
}

shared_ptr<SymbolTable> SymbolTable::delete_scope() {
	return m_enclosing_scope;
}
