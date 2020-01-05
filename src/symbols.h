#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#include <string>
#include <memory>
#include <utility>
#include <unordered_map>
#include <variant>

using std::string;
using std::shared_ptr;
using std::pair;
using std::unordered_map;

enum value_type {
	SV_STRING,
	SV_INT,
	SV_CHAR
};

class SymbolTable : public std::enable_shared_from_this<SymbolTable> {
public:
	void add_symbol(string symb, int n);
	void add_symbol(string symb, string s);
	void add_symbol(string symb, char c);
	void set_enclosing_scope(shared_ptr<SymbolTable> S) { m_enclosing_scope = S; }
	pair<std::variant<int, char, string>,bool> get_symbol(string symb);
	shared_ptr<SymbolTable> create_scope();
private:
	std::weak_ptr<SymbolTable> m_enclosing_scope;
	unordered_map<string, std::variant<int, char, string>> m_map;
};

#endif /* __SYMBOL_H__ */
