#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#include <string>
#include <memory>
#include <utility>
#include <unordered_map>
#include <variant>
#include <iostream>

using std::string;
using std::shared_ptr;
using std::pair;
using std::unordered_map;

typedef std::variant<int, char, std::string> scVal;

class SymbolTable : public std::enable_shared_from_this<SymbolTable> {
public:
	void add_symbol(string symb, scVal v);
	void set_enclosing_scope(shared_ptr<SymbolTable> S) { m_enclosing_scope = S; }
	pair<scVal,bool> lookup_symbol(string symb);
	shared_ptr<SymbolTable> create_scope();
	shared_ptr<SymbolTable> delete_scope();
private:
	std::shared_ptr<SymbolTable> m_enclosing_scope;
	unordered_map<string, scVal> m_map;
};

#endif /* __SYMBOL_H__ */
