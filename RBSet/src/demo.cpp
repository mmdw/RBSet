#include <iostream>
#include <sstream>

#include "util.h"
#include "RBTree/RbParser.h"
#include "RBTree/RBTree.h"

using namespace std;

typedef Node<int> TreeNode;

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
	Node<int> * p_node = NULL;

	parseRbNode(iss, &p_node);
	p_node->serialize(std::cout, 0);
}

void testIterator() {
	RBTree<int> tree;
	tree.put(1);
	tree.put(4);
	tree.put(2);
	tree.put(3);

	RBTree<int>::Iterator iter = tree.begin();
	cout << *iter << std::endl; // 1
	++iter;

	cout << *iter << std::endl; // 2
	++iter;

	cout << *iter << std::endl; // 3
	++iter;

	cout << *iter << std::endl; // 4
	++iter;

	cout << "---" << std::endl;

	--iter;
	cout << *iter << std::endl; // 4

	--iter;
	cout << *iter << std::endl; // 3

	--iter;
	cout << *iter << std::endl; // 2

	--iter;
	cout << *iter << std::endl; // 1
}

void testIteratorComparsion() {
	RBTree<int> tree;
	tree.put(5);
	tree.put(6);
	tree.put(7);
	tree.put(3);
	tree.put(2);
	tree.put(1);
	tree.put(4);

	// forward
	for (RBTree<int>::Iterator it = tree.begin(); it != tree.end(); ++it) {
		cout << *it << " ";
	}
	cout << endl;

	// backward
	RBTree<int>::Iterator it = tree.end();
	while (it != tree.begin()) {
		--it;
		cout << *it << " ";
	}
	cout << endl;
}

void testIteratorFind() {
	RBTree<int> tree;
	tree.put(5);
	tree.put(6);
	tree.put(7);
	tree.put(3);
	tree.put(2);
	tree.put(1);
	tree.put(4);

	cout << (*tree.find(6) == 6) << std::endl;
	cout << (*tree.find(2) == 2) << std::endl;
	cout << (tree.find(42) == tree.end()) << std::endl;
}

int main() {
	testIteratorFind();
	return 0;
}
