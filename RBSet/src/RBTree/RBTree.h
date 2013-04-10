/*
 * RBTree.h
 *
 *  Created on: 03.03.2013
 *      Author: user
 */

#ifndef RBTREE_H_
#define RBTREE_H_

#include <ostream>
#include "RBNode.h"

using std::ostream;
using std::istream;
using std::endl;

template <typename T>
class RBTree {
protected:
	Node<T> * p_root;
	size_t count;

public:
	RBTree() : p_root(NULL), count(0) {
	}

	RBTree(const RBTree& rhs) : count (rhs.count) {
		p_root = copyNode(rhs.p_root);
	}

	RBTree(istream& os);

	void put(const T& key);
	void serialize(ostream& os);
	size_t size() const;

	virtual ~RBTree() {
		delete p_root;
	}

	void erase(const T& key);
};

template<typename T>
RBTree<T>::RBTree(istream& is) : p_root(NULL), count(0) {
	parseRbNode(is, &p_root, count);
}

template<typename T>
void RBTree<T>::serialize(ostream& os) {
	if (p_root == NULL) {
		os << "null";
	} else {
		p_root->serialize(os, 0);
	}
}

template <typename T>
size_t RBTree<T>::size() const {
	return count;
}

template <typename T>
void RBTree<T>::put(const T& key) {
	if (rbTreeInsert(&p_root, key)) {
		count++;
	}
}

template <typename T>
void RBTree<T>::erase(const T& key) {
	Node<T>* p_node = p_root;

	while (p_node != NULL && p_node->key != key) {
		p_node = p_node->key > key ? p_node->left : p_node->right;
	}

	if (p_node != NULL) {
		rbDelete(&p_root, p_node);
		--count;
	}
}

#endif /* RBTREE_H_ */
