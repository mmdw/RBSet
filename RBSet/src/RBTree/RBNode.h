#pragma once

#include <algorithm>
#include <ostream>

using std::ostream;
using std::istream;

namespace Tree {
	enum NodeColor {
		RED, BLACK
	};
			
	template <typename T>
	struct Node {
		typedef std::allocator<Node> NodeAllocator;

		T key;

		typename NodeAllocator::pointer parent;
		typename NodeAllocator::pointer left;
		typename NodeAllocator::pointer right;

		NodeColor color;

		Node(const T& key) :
			key(key), parent(NULL), left(NULL), right(NULL), color(BLACK) {
		}

		Node() :
			parent(NULL), left(NULL), right(NULL), color(BLACK) {
		}

		~Node() {

		}

		void serialize(ostream& os, int tabs);
	};

	template <typename T, typename ItemAlloc>
	typename Node<T>::NodeAllocator::pointer 
	copyNode(const Node<T>* rhs, typename Node<T>::NodeAllocator& nodeAlloc, ItemAlloc& itemAlloc) {
		if (rhs == NULL) {
			return NULL;
		}

		Node<T>::NodeAllocator::pointer result = nodeAlloc.allocate(1);
		itemAlloc.construct(result, ItemAlloc::value_type());
		

		result->color = rhs->color;
		result->key = rhs->key;

		if (rhs->left != NULL) {
			result->left = copyNode(rhs->left, nodeAlloc, itemAlloc);
			result->left->parent = result;
		}

		if (rhs->right != NULL) {
			result->right = copyNode(rhs->right, nodeAlloc, itemAlloc);
			result->right->parent = result;
		}

		return result;
	}

	template <typename T>
	void leftRotate(typename Node<T>::NodeAllocator::pointer * pp_root, typename Node<T>::NodeAllocator::pointer p_x) {
		Node<T>::NodeAllocator::pointer p_y = p_x->right;

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
	void rightRotate(typename Node<T>::NodeAllocator::pointer * pp_root, typename Node<T>::NodeAllocator::pointer p_y) {
		typename Node<T>::NodeAllocator::pointer p_x = p_y->left;

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
	void rbDelete(Node<T>** pp_root, Node<T>* p_z) {
		Node<T>* p_y = NULL;
		if (p_z->left == NULL || p_z->right == NULL) {
			p_y = p_z;
		} else {
			p_y = treeSuccessor(p_z);
		}

		Node<T>* p_x = NULL;
		if (p_y->left != NULL) {
			p_x = p_y->left;
		} else {
			p_x = p_y->right;
		}

		Node<T>* p_x_parent = NULL;
		if (p_x != NULL) {
			p_x->parent = p_x_parent = p_y->parent;
		} else {
			p_x_parent = p_y->parent;
		}

		if (p_y->parent == NULL) {
			*pp_root = p_x;
		} else {
			if (p_y == p_y->parent->left){
				p_y->parent->left = p_x;
			} else {
				p_y->parent->right = p_x;
			}
		}

		if (p_y != p_z) {
			p_z->key = p_y->key;
		}

		if (p_y->color == BLACK) {
			rbDeleteFixup(pp_root, p_x, p_x_parent);
		}
	}

	template <typename T>
	void rbDeleteFixup(Node<T>** root, Node<T>* x, Node<T>* x_parent) {
		while (x != *root && color(x) == BLACK) {
			if (x == x_parent->left) {
				Node<T>* w = NULL;

				w = x_parent->right;

				if (color(w) == RED) {
					w->color = BLACK;
					x_parent->color = RED;
					leftRotate<T>(root, x_parent);
					w = x_parent->right;
				}

				if (color(w->left)== BLACK && color(w->right) == BLACK) {
					w->color = RED;
					x = x_parent;
				} else {
					if (color(w->right) == BLACK) {
						w->left->color = BLACK;
						w->color = RED;

						rightRotate<T>(root, w);
						w = x_parent->right;
					}

					w->color = x_parent->color;
					x_parent->color = BLACK;
					w->right->color = BLACK;
					leftRotate<T>(root, x_parent);
					x = *root;
				}
			} else {
				Node<T>* w = NULL;

				w = x_parent->left;

				if (color(w) == RED) {
					w->color = BLACK;
					x_parent->color = RED;
					rightRotate<T>(root, x_parent);
					w = x_parent->left;
				}

				if (color(w->left)== BLACK && color(w->right) == BLACK) {
					w->color = RED;
					x = x_parent;
				} else {
					if (color(w->right) == BLACK) {
						w->left->color = BLACK;
						w->color = RED;

						leftRotate<T>(root, w);
						w = x_parent->left;
					}

					w->color = x_parent->color;
					x_parent->color = BLACK;
					w->left->color = BLACK;
					rightRotate<T>(root, x_parent);
					x = *root;
				}
			}
		}

		if (x != NULL) {
			x->color = BLACK;
		}
	}

	template <typename T>
	Node<T>* treeMinimum(Node<T>* p_x) {
		while (p_x->left != NULL) {
			p_x = p_x->left;
		}

		return p_x;
	}

	template <typename T>
	Node<T>* treeSuccessor(Node<T>* p_x) {
		if (p_x->right != NULL) {
			return treeMinimum(p_x->right);
		}

		Node<T>* p_y = p_x->parent;

		while (p_y != NULL && p_x == p_y->right) {
			p_x = p_y;
			p_y = p_y->parent;
		}

		return p_y;
	}

	template <typename T, typename NodeAlloc, typename ItemAlloc>
	typename NodeAlloc::pointer treeNewNode(const T& value,
		NodeAlloc& nodeAlloc, ItemAlloc& itemAlloc) {

		ItemAlloc::pointer item = itemAlloc.allocate(1);
		itemAlloc.construct(item, ItemAlloc::value_type(value));

		NodeAlloc::pointer node = nodeAlloc.allocate(1);
		nodeAlloc.construct(node, NodeAlloc::value_type(*item));

		return node;
	}

	template <typename T, typename ItemAlloc>
	typename Node<T>::NodeAllocator::pointer treeInsert(
		typename Node<T>::NodeAllocator::pointer * pp_root, const T& value, 
		typename Node<T>::NodeAllocator& nodeAlloc, ItemAlloc& itemAlloc) {

		if (*pp_root == NULL) {
			*pp_root = treeNewNode(value, nodeAlloc, itemAlloc);

			return *pp_root;
		}

		Node<T>::NodeAllocator::pointer * pp_node = pp_root;
		Node<T>::NodeAllocator::pointer p_parent = NULL;

		while (*pp_node != NULL) {
			if ((*pp_node)->key == value) {
				return NULL;
			}

			p_parent = *pp_node;

			if (value < (*pp_node)->key) {
				pp_node = &((*pp_node)->left);
			} else {
				pp_node = &((*pp_node)->right);
			}
		}

		*pp_node = treeNewNode(value, nodeAlloc, itemAlloc);
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

	template <typename T, typename ItemAlloc>
	bool rbTreeInsert(typename Node<T>::NodeAllocator::pointer * pp_root, const T& key,
		typename Node<T>::NodeAllocator& nodeAlloc, ItemAlloc& itemAlloc) {

		Node<T>::NodeAllocator::pointer x = treeInsert(pp_root, key, nodeAlloc, itemAlloc);

		if (x == NULL) {
			return false;
		}

		x->color = RED;

		while (x != *pp_root && x->parent->color == RED) {
			if (x->parent == x->parent->parent->left) {
				Node<T>::NodeAllocator::pointer y = x->parent->parent->right;

				if (color(y) == RED) {
					x->parent->color = BLACK;
					y->color = BLACK;
					x->parent->parent->color = RED;

					x = x->parent->parent;
				} else if (x == x->parent->right) {
					x = x->parent;
					leftRotate<T>(pp_root, x);
				}

				if (x->parent != NULL) {
					x->parent->color = BLACK;

					if (x->parent->parent != NULL) {
						x->parent->parent->color = RED;

						rightRotate<T>(pp_root, x->parent->parent);
					}
				}
			} else {
				Node<T>::NodeAllocator::pointer y = x->parent->parent->left;

				if (color(y) == RED) {
					x->parent->color = BLACK;
					y->color = BLACK;
					x->parent->parent->color = RED;

					x = x->parent->parent;
				} else if (x == x->parent->left) {
					x = x->parent;
					rightRotate<T>(pp_root, x);
				}

				if (x->parent != NULL) {
					x->parent->color = BLACK;

					if (x->parent->parent != NULL) {
						x->parent->parent->color = RED;

						leftRotate<T>(pp_root, x->parent->parent);
					}
				}	
			}
		}

		(*pp_root)->color = BLACK;

		return true;
	}

	template <typename T, typename ItemAlloc>
	void destroy(typename Node<T>::NodeAllocator::pointer& p_node, 
		typename Node<T>::NodeAllocator& nodeAlloc, ItemAlloc& itemAlloc) {

		if (p_node != NULL) {
			if (p_node->left != NULL) {
				destroy<T>(p_node->left, nodeAlloc, itemAlloc);
			}

			if (p_node->right != NULL) {
				destroy<T>(p_node->right, nodeAlloc, itemAlloc);
			}
		}

		nodeAlloc.destroy(p_node);
	}

	template<typename T>
	void Node<T>::serialize(ostream& os, int tabs) {
		std::string newline;
		newline += '\n';

		for (int i = 0; i < 2 * tabs; ++i) {
			newline += ' ';
		}

		++tabs;

		os << "{";
		os << newline;

		os << "key: " << key << ",";
		os << newline;

		os << "color: " << (color == BLACK ? "'black'" : "'red'") << ",";
		os << newline;

		os << "left: ";

		if (left == NULL) {
			os << "null";
		} else {
			left->serialize(os, tabs);
		}

		os << ",";

		os << newline;
		os << "right: ";

		if (right == NULL) {
			os << "null";
		} else {
			right->serialize(os, tabs);
		}

		--tabs;
		os << newline;
		os << "}";
	}
}
