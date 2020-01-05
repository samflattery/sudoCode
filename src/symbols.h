#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#include <variant>
#include <string>

typedef std::variant<int, char, float> value_type ;

typedef std::variant<std::string, void*> alloc_type ;

typedef std::variant<value_type, alloc_type> scVal;


#endif /* __SYMBOL_H__ */
