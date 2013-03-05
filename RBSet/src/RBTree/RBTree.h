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
	Node<T> * p_root;
	size_t count;

public:
	class Iterator;

	RBTree() : p_root(NULL), count(0) {
	}

	RBTree(istream& os);

	void put(const T& key);
	void serialize(ostream& os);
	size_t size();

	virtual ~RBTree() {
		delete p_root;
	}

	Iterator begin();
	Iterator end();
	Iterator find(const T& key);

	class Iterator {
		friend Iterator RBTree<T>::begin();
		friend Iterator RBTree<T>::end();
		friend Iterator RBTree<T>::find(const T& key);

		Node<T>* p_node;
		Node<T>* p_root;

		Iterator(Node<T>* p_node, Node<T>* p_root);

	public:
		virtual ~Iterator();

		Iterator& operator=(const Iterator&);
		bool operator==(const Iterator&);
		bool operator!=(const Iterator&);

		Iterator& operator++();
		Iterator& operator--();

		T& operator*();
	};
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
size_t RBTree<T>::size() {
	return count;
}

template <typename T>
void RBTree<T>::put(const T& key) {
	if (rbTreeInsert(&p_root, key)) {
		count++;
	}
}

template <typename T>
typename RBTree<T>::Iterator RBTree<T>::begin() {
	Node<T>* p_node = p_root;
	while (p_node->left != NULL) {
		p_node = p_node->left;
	}

	return Iterator(p_node, p_root);
}

template <typename T>
typename RBTree<T>::Iterator RBTree<T>::end() {
	return Iterator(NULL, p_root);
}

template <typename T>
typename RBTree<T>::Iterator RBTree<T>::find(const T& key) {
	Node<T>* p_node = p_root;

	while (p_node != NULL && p_node->key != key) {
		p_node = p_node->key > key ? p_node->left : p_node->right;
	}

	return Iterator(p_node, p_root);
}

template <typename T>
typename RBTree<T>::Iterator& RBTree<T>::Iterator::operator++() {
	// up
	if (p_node == NULL) {
		std::cerr << "end reached";
		exit(-1);
	}

	if (p_node->right != NULL) {
		p_node = p_node->right;

		while (p_node->left != NULL) {
			p_node = p_node->left;
		}
	} else {
		while (p_node->parent != NULL && p_node == p_node->parent->right) {
			p_node = p_node->parent;
		}

		p_node = p_node->parent;
	}

	return *this;
}

template <typename T>
typename RBTree<T>::Iterator& RBTree<T>::Iterator::operator--() {
	// up
	if (p_node == NULL && p_root == NULL) {
		std::cerr << "empty tree";
		exit(-1);
	}

	if (p_node == NULL) {
		p_node = p_root;
		while (p_node->right != NULL) {
			p_node = p_node->right;
		}
	} else {
		if (p_node->left != NULL) {
			p_node = p_node->left;

			while (p_node->right != NULL) {
				p_node = p_node->right;
			}
		} else {
			while (p_node->parent != NULL && p_node == p_node->parent->left) {
				p_node = p_node->parent;
			}

			p_node = p_node->parent;
		}
	}

	return *this;
}

template <typename T>
T& RBTree<T>::Iterator::operator*() {
	// todo NULL dereferencing
	return p_node->key;
}

template <typename T>
bool RBTree<T>::Iterator::operator==(const Iterator& iter) {
	return iter.p_root == p_root && iter.p_node == p_node;
}

template <typename T>
bool RBTree<T>::Iterator::operator!=(const Iterator& iter) {
	return !(*this == iter);
}

template <typename T>
RBTree<T>::Iterator::Iterator(Node<T>* p_node, Node<T>* p_root)
	: p_node(p_node), p_root(p_root) {

}

template <typename T>
RBTree<T>::Iterator::~Iterator() {

}
#endif /* RBTREE_H_ */
