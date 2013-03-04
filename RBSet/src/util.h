/*
 * util.h
 *
 *  Created on: 02.03.2013
 *      Author: user
 */

#ifndef UTIL_H_
#define UTIL_H_

#include "RBTree/RBNode.h"

#include <iostream>
#include <cassert>

template <typename T>
void print_node(std::ostream& os, Node<T>* node) {
	if (node == NULL) {
		os << "nil";
	} else {
		os << "(" << node->key;

		if (node->color == RED) {
			os << " -R- ";
		} else {
			os << " -B- ";
		}

		print_node(os, node->left);
		os << " # ";
		print_node(os, node->right);
		os << ")";
	}
}

template <typename T>
void debug(Node<T>* root) {
	print_node(std::cout, root);
	std::cout << std::endl;
}

template <typename T>
void checkParent(Node<T>* node) {
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

#endif /* UTIL_H_ */
