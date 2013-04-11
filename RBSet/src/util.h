#pragma once

#include "RBTree/RBNode.h"

#include <iostream>
#include <cassert>
#include <set>
#include <list>

using std::set;
using std::list;

template <typename T>
void print_node(std::ostream& os, Tree::Node<T>* node) {
	if (node == NULL) {
		os << "nil";
	} else {
		os << "(" << node->key;

		if (node->color == Tree::RED) {
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
void debug(Tree::Node<T>* root) {
	print_node(std::cout, root);
	std::cout << std::endl;
}

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
