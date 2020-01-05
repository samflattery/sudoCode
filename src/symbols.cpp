#include "symbols.h"

void SymbolTable::add_symbol(string symb, int n) {
	std::variant<int, char, string> v;
	v = n;
	m_map[symb] = v;
}

void SymbolTable::add_symbol(string symb, string s) {
	std::variant<int, char, string> v;
	v = s;
	m_map[symb] = v;
}

void SymbolTable::add_symbol(string symb, char c) {
	std::variant<int, char, string> v;
	v = c;
	m_map[symb] = v;
}

pair<std::variant<int, char, string>, bool> SymbolTable::get_symbol(string symb) {
	pair<std::variant<int, char, string>, bool> p;
	if (m_map.find(symb) != m_map.end()) {
		p.first = m_map[symb];
		p.second = true;
		return p;
	}
	std::variant<int, char, string> v = 0;
	p.first = v;
	p.second = false;
	return p;
}

shared_ptr<SymbolTable> SymbolTable::create_scope() {
	shared_ptr<SymbolTable> ST = std::make_shared<SymbolTable>();
	ST->set_enclosing_scope(shared_from_this());
	return ST;
}
