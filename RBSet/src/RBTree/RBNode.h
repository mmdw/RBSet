/*
 * rbtree.h
 *
 *  Created on: 02.03.2013
 *      Author: user
 */

#ifndef RBNODE_H_
#define RBNODE_H_

#include <algorithm>
#include <ostream>

using std::ostream;
using std::istream;

enum NodeColor {
	RED, BLACK
};

ostream& operator<<(ostream& os, const NodeColor& color);
void newline(ostream& os, int tabs);

template <typename T>
struct Node {
	T key;

	Node* parent;
	Node* left;
	Node* right;
	NodeColor color;

	Node(const T& key) :
		key(key), parent(NULL), left(NULL), right(NULL), color(BLACK) {
	}

	Node() :
		parent(NULL), left(NULL), right(NULL), color(BLACK) {
	}

	~Node() {
		delete left;
		delete right;
	}

	void serialize(ostream& os, int tabs);
};

template <typename T>
void leftRotate(Node<T>** pp_root, Node<T>* p_x) {
	Node<T>* p_y = p_x->right;

	p_x->right = p_y->left;
	if (p_x->right != NULL) {
		p_x->right->parent = p_x;
	}

	p_y->parent = p_x->parent;

	if (p_x->parent != NULL) {
		if (p_x == p_x->parent->left) {
			p_x->parent->left = p_y;
		} else {
			p_x->parent->right = p_y;
		}
	} else {
		(*pp_root) = p_y;
	}

	p_y->left = p_x;
	p_x->parent = p_y;
}

template <typename T>
void rightRotate(Node<T>** pp_root, Node<T>* p_y) {
	Node<T>* p_x = p_y->left;

	p_y->left= p_x->right;
	if (p_y->left != NULL) {
		p_y->left->parent = p_y;
	}

	p_x->parent = p_y->parent;

	if (p_y->parent != NULL) {
		if (p_y == p_y->parent->right) {
			p_y->parent->right = p_x;
		} else {
			p_y->parent->left = p_x;
		}
	} else {
		(*pp_root) = p_x;
	}

	p_x->right= p_y;
	p_y->parent = p_x;
}

template <typename T>
Node<T>* treeInsert(Node<T>** pp_root, const T& value) {
	if (*pp_root == NULL) {
		*pp_root = new Node<T>(value);

		return *pp_root;
	}

	Node<T>** pp_node = pp_root;
	Node<T>* p_parent = NULL;

	while (*pp_node != NULL) {
		if ((*pp_node)->key == value) {
			return NULL;
		}

		p_parent = *pp_node;

		if (value < (**pp_node).key) {
			pp_node = &((**pp_node).left);
		} else {
			pp_node = &((**pp_node).right);
		}
	}

	*pp_node = new Node<T>(value);
	(*pp_node)->parent = p_parent;

	return *pp_node;
}

template <typename T>
NodeColor color(Node<T>* node) {
	if (node == NULL) {
		return BLACK;
	}

	return node->color;
}

template <typename T>
void rbTreeInsert(Node<T>** pp_root, const T& key) {
	Node<T>* x = treeInsert(pp_root, key);

	if (x == NULL) {
		return;
	}

	x->color = RED;

	while (x != *pp_root && x->parent->color == RED) {
		if (x->parent == x->parent->parent->left) {
			Node<T>* y = x->parent->parent->right;

			if (color(y) == RED) {
				x->parent->color = BLACK;
				y->color = BLACK;
				x->parent->parent->color = RED;

				x = x->parent->parent;
			} else if (x == x->parent->right) {
				x = x->parent;
				leftRotate(pp_root, x);
			}

			if (x->parent != NULL) {
				x->parent->color = BLACK;

				if (x->parent->parent != NULL) {
					x->parent->parent->color = RED;

					rightRotate(pp_root, x->parent->parent);
				}
			}
		} else {
			Node<T>* y = x->parent->parent->left;

			if (color(y) == RED) {
				x->parent->color = BLACK;
				y->color = BLACK;
				x->parent->parent->color = RED;

				x = x->parent->parent;
			} else if (x == x->parent->left) {
				x = x->parent;
				rightRotate(pp_root, x);
			}

			if (x->parent != NULL) {
				x->parent->color = BLACK;

				if (x->parent->parent != NULL) {
					x->parent->parent->color = RED;

					leftRotate(pp_root, x->parent->parent);
				}
			}
		}
	}

	(*pp_root)->color = BLACK;
}

template<typename T>
void Node<T>::serialize(ostream& os, int tabs) {
	++tabs;

	os << "{";
	newline(os, tabs);

	os << "key: " << key << ",";
	newline(os, tabs );

	os << "color: " << color << ",";
	newline(os, tabs);

	os << "left: ";

	if (left == NULL) {
		os << "null";
	} else {
		left->serialize(os, tabs);
	}

	os << ",";

	newline(os, tabs);
	os << "right: ";

	if (right == NULL) {
		os << "null";
	} else {
		right->serialize(os, tabs);
	}

	--tabs;
	newline(os, tabs);
	os << "}";
}
#endif
