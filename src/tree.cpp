#include "tree.h"

/*
 * show_tree:
 * ----------
 * writes a graphviz graph to a given filename
 */
void Tree::show_tree(const char filename[]) {
	std::ofstream fout;
	fout.open(filename, std::ios::out | std::ios::trunc);
	fout << "digraph g{" << endl;
	fout << " node [shape = record,height=.1];" << endl;

	vector<shared_ptr<Tree>> queue;
	size_t id = 0;

	/* perform a BFS of the tree to generate the nodes */
	/* need to store pointers, but can't make a pointer out of self without
	 * creating a cycle, so just put nullptr on the stack to signify the first
	 * iteration
	 */
	queue.push_back(nullptr);
	while (!queue.empty()) {
		shared_ptr<Tree> P = queue.front();
		queue.erase(queue.begin());
		if (P == nullptr) {
			set_id(id);
			id++;
			shared_ptr<vector<shared_ptr<Tree>>> desc = get_descendents();
			for (auto x : *desc) {
				queue.push_back(x);
			}
			generate_vertex(fout);
			continue;
		}
		P->set_id(id);
		id++;
		shared_ptr<vector<shared_ptr<Tree>>> desc = P->get_descendents();
		for (auto x : *desc) {
			queue.push_back(x);
		}
		P->generate_vertex(fout);
	}

	/* generate the edges with another BFS */
	queue.push_back(nullptr);
	while (!queue.empty()) {
		shared_ptr<Tree> P = queue.front();
		queue.erase(queue.begin());
		if (P == nullptr) {
			shared_ptr<vector<shared_ptr<Tree>>> desc = get_descendents();
			for (auto x : *desc) {
				queue.push_back(x);
			}
			generate_edge(fout);
			continue;
		}
		shared_ptr<vector<shared_ptr<Tree>>> desc = P->get_descendents();
		for (auto x : *desc) {
			queue.push_back(x);
		}
		P->generate_edge(fout);
	}

	fout << "}" << std::endl;
}
