#include <iostream>
#include <sstream>
#include <cassert>
#include <list>
#include <set>

#include "RBTree/RBNodeParser.h"
#include "RBTree/RBTree.h"
#include "RBSet.h"

using namespace std;

typedef Tree::Node<int> TreeNode;

template <typename T>
list<list<T> > producePermutations(const std::set<T>& s) {
	list<list<T> > result;

	if (s.size() == 1) {
		result.push_back(list<T>(s.begin(), s.end()));
	} else {
		for (typename set<T>::iterator it1 = s.begin(); it1 != s.end(); ++it1) {
			set<T> ds(s);
			ds.erase(*it1);

			list<list<T> > p = producePermutations(ds);
			for (typename list<list<T> >::iterator it2 = p.begin(); it2 != p.end(); ++it2) {
				it2->push_front(*it1);
			}

			result.insert(result.end(), p.begin(), p.end());
		}
	}

	return result;
}

template <typename T>
void checkParent(Tree::Node<T>* node) {
	if (node != NULL) {
		if (node->left != NULL) {
			assert(node->left->parent == node);
			checkParent(node->left);
		}

		if (node->right != NULL) {
			assert(node->right->parent == node);
			checkParent(node->right);
		}
	}
}
void testParser() {
	istringstream iss("{key: 13, color: red, left:null, right : {key: 99, color: red, left: {key: 77}}}");
	Tree::Node<int> * p_node = NULL;
	size_t count = 0;

	Tree::RBNodeParser p(iss);
	p.parseRbNode(&p_node, count);
	assert(count == 3);

	p_node->serialize(std::cout, 0);
}

void testRepeatedInsert() {
	Tree::RBTree<int> tree;
	tree.put(3);
	tree.put(2);
	tree.put(1);
	assert(tree.size() == 3);

	tree.put(2);
	tree.put(3);
	tree.put(1);
	assert(tree.size() == 3);

	tree.serialize(cout);
}

void testInsertPermutations() {
	set<int> s;
	const unsigned n = 6;
	for (unsigned i = 1; i <= n; ++i) {
		s.insert(i);
	}

	list<list<int> > ps = producePermutations(s);
	for (list<list<int> >::const_iterator it1 = ps.begin(); it1 != ps.end(); ++it1) {
		Tree::RBTree<int> tree;
		for (list<int>::const_iterator it2 = it1->begin(); it2 != it1->end(); ++it2) {
			tree.put(*it2);
		}

		assert(tree.size() == n);
	}
}

void testCopy() {
	Tree::RBTree<int> tree;
	tree.put(3);
	tree.put(2);
	tree.put(1);
	tree.put(4);

	Tree::RBTree<int> tree1(tree);
	assert(tree1.size() == tree.size());
}


void testRbSet() {
	RBSet<int> set;
	set.put(3);
	set.put(2);
	set.put(1);
	set.put(4);

	set.remove(3);

	AbstractIterator<int>* it = set.iterator();
	while (it->hasNext()) {
		std::cout << it->item() << " ";
		it->next();
	}
	delete it;
}
int main() {
	testRbSet();
	return 0;
}
