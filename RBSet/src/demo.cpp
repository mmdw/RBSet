#include <iostream>
#include <sstream>

#include "util.h"
#include "RBTree/RbParser.h"
#include "RBTree/RBTree.h"
#include "RBSet.h"

using namespace std;

typedef Tree::Node<int> TreeNode;

void testDebug() {
	TreeNode* p_a = new TreeNode(13);
	treeInsert(&p_a, 16);
	treeInsert(&p_a, 10);

	debug(p_a);
}

void testRotate() {
	TreeNode* p_a = new TreeNode(5);
	checkParent(p_a);

	treeInsert(&p_a, 4);
	treeInsert(&p_a, 7);
	treeInsert(&p_a, 6);
	treeInsert(&p_a, 8);

	debug(p_a);
	rightRotate(&p_a, p_a);
	checkParent(p_a);

	debug(p_a);
	leftRotate(&p_a, p_a);
	checkParent(p_a);

	debug(p_a);

	checkParent(p_a);
}

void testRbInsert() {
	TreeNode* p_a = NULL;

	rbTreeInsert(&p_a, 6);
	debug(p_a);

	rbTreeInsert(&p_a, 7);
	debug(p_a);

	rbTreeInsert(&p_a, 10);
	debug(p_a);

	rbTreeInsert(&p_a, 11);
	debug(p_a);

	rbTreeInsert(&p_a, 12);
	debug(p_a);

	rbTreeInsert(&p_a, 8);
	debug(p_a);

	rbTreeInsert(&p_a, 9);
	debug(p_a);

	rbTreeInsert(&p_a, 3);
	debug(p_a);

	rbTreeInsert(&p_a, 4);
	debug(p_a);

	rbTreeInsert(&p_a, 2);
	debug(p_a);
}

void testParser() {
	istringstream iss("{key: 13, color: red, left:null, right : {key: 99, color: red, left: {key: 77}}}");
	Tree::Node<int> * p_node = NULL;
	size_t count = 0;

	Tree::parseRbNode(iss, &p_node, count);
	assert(count == 3);

	p_node->serialize(std::cout, 0);
}

//void testIterator() {
//	RBTree<int> tree;
//	tree.put(1);
//	tree.put(4);
//	tree.put(2);
//	tree.put(3);
//
//	RBTree<int>::Iterator iter = tree.begin();
//	cout << *iter << std::endl; // 1
//	++iter;
//
//	cout << *iter << std::endl; // 2
//	++iter;
//
//	cout << *iter << std::endl; // 3
//	++iter;
//
//	cout << *iter << std::endl; // 4
//	++iter;
//
//	cout << "---" << std::endl;
//
//	--iter;
//	cout << *iter << std::endl; // 4
//
//	--iter;
//	cout << *iter << std::endl; // 3
//
//	--iter;
//	cout << *iter << std::endl; // 2
//
//	--iter;
//	cout << *iter << std::endl; // 1
//}

//void testIteratorComparsion() {
//	RBTree<int> tree;
//	tree.put(5);
//	tree.put(6);
//	tree.put(7);
//	tree.put(3);
//	tree.put(2);
//	tree.put(1);
//	tree.put(4);
//
//	// forward
//	for (RBTree<int>::Iterator it = tree.begin(); it != tree.end(); ++it) {
//		cout << *it << " ";
//	}
//	cout << endl;
//
//	// backward
//	RBTree<int>::Iterator it = tree.end();
//	while (it != tree.begin()) {
//		--it;
//		cout << *it << " ";
//	}
//	cout << endl;
//}
//
//void testIteratorFind() {
//	RBTree<int> tree;
//	tree.put(5);
//	tree.put(6);
//	tree.put(7);
//	tree.put(3);
//	tree.put(2);
//	tree.put(1);
//	tree.put(4);
//
//	assert(*tree.find(6) == 6);
//	assert(*tree.find(2) == 2);
//	assert(tree.find(42) == tree.end());
//}

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

//void testRemove() {
//	RBTree<int> tree;
//	tree.put(3);
//	tree.put(2);
//	tree.put(1);
//	tree.put(4);
//
//	tree.erase(1);
//	tree.erase(2);
//	tree.erase(4);
//	tree.erase(3);
//
//	tree.serialize(cout);
//	for (RBTree<int>::Iterator it = tree.begin(); it != tree.end(); ++it) {
//		cout << *it << " ";
//	}
//	cout << endl;
//}

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
//	assert(tree1.find(1) != tree1.end());
//	assert(tree1.find(2) != tree1.end());
//	assert(tree1.find(3) != tree1.end());
//	assert(tree1.find(4) != tree1.end());
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
