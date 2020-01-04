#ifndef __TREE_H__
#define __TREE_H__

#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include <vector>
#include "tokens.h"
#include "grammar.h"

using std::shared_ptr;
using std::endl;
using std::vector;

class Tree {
public:
	void show_tree(const char filename[]);
	virtual void generate_edge(std::ofstream &fout) = 0;
	virtual void generate_vertex(std::ofstream &fout) = 0;
	virtual shared_ptr<vector<shared_ptr<Tree>>> get_descendents() = 0;
	virtual void set_id(size_t id) = 0;
	virtual size_t get_id() = 0;
};

#endif /* __TREE_H__ */
