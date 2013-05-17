#pragma once

#include "ItemArray.h"

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
		T key;

		typename ItemArray<Node<T> >::ItemId parent;
		typename ItemArray<Node<T> >::ItemId left;
		typename ItemArray<Node<T> >::ItemId right;

		NodeColor color;

		Node(const T& key) :
			key(key),
			parent(ItemArray<Node<T> >::Null),
			left(ItemArray<Node<T> >::Null),
			right(ItemArray<Node<T> >::Null),
			color(BLACK) {
		}

		Node() :
			parent(ItemArray<Node<T> >::Null),
			left(ItemArray<Node<T> >::Null),
			right(ItemArray<Node<T> >::Null),
			color(BLACK) {
		}

		~Node() {

		}

		void serialize(ostream& os, int tabs, ItemArray<Node<T> >& ia);
	};

	template <typename T>
	typename Node<T>::NodeAllocator::pointer 
	copyNode(const Node<T>* rhs, ItemArray< Node <T> >& ia) {
		if (rhs == NULL) {
			return NULL;
		}

		typename ItemArray<Node<T> >::ItemId result = ia.alloc(Node<T>());

		ia[result].color = rhs->color;
		ia[result].key   = rhs->key;

		if (rhs->left != NULL) {
			ia[result].left = copyNode(rhs->left, ia);
			ia[ia[result].left].parent = result;
		}

		if (rhs->right != NULL) {
			ia[result].right = copyNode(rhs->right, ia);
			ia[ia[result].right].parent = result;
		}

		return result;
	}

	template <typename T>
	void leftRotate(typename ItemArray< Node<T> >::ItemId * pp_root,
			        typename ItemArray< Node<T> >::ItemId p_x, ItemArray< Node<T> >& ia) {

		unsigned p_y = ia[p_x].right;

		ia[p_x].right = ia[p_y].left;
		if (ia[p_x].right != ItemArray<Node<T> >::Null) {
			ia[ia[p_x].right].parent = p_x;
		}

		ia[p_y].parent = ia[p_x].parent;

		if (ia[p_x].parent != ItemArray<Node<T> >::Null) {
			if (p_x == ia[ia[p_x].parent].left) {
				ia[ia[p_x].parent].left = p_y;
			} else {
				ia[ia[p_x].parent].right = p_y;
			}
		} else {
			(*pp_root) = p_y;
		}

		ia[p_y].left = p_x;
		ia[p_x].parent = p_y;
	}

	template <typename T>
	void rightRotate(typename ItemArray< Node<T> >::ItemId * pp_root,
					 typename ItemArray< Node<T> >::ItemId p_y, ItemArray< Node<T> >& ia) {
		unsigned p_x = ia[p_y].left;

		ia[p_y].left = ia[p_x].right;
		if (ia[p_y].left != ItemArray<Node<T> >::Null) {
			ia[ia[p_y].left].parent = p_y;
		}

		ia[p_x].parent = ia[p_y].parent;

		if (ia[p_y].parent != ItemArray<Node<T> >::Null) {
			if (p_y == ia[ia[p_y].parent].right) {
				ia[ia[p_y].parent].right = p_x;
			} else {
				ia[ia[p_y].parent].left = p_x;
			}
		} else {
			(*pp_root) = p_x;
		}

		ia[p_x].right= p_y;
		ia[p_y].parent = p_x;
	}

	template <typename T>
	void rbDelete(typename ItemArray<Node<T> >::ItemId * pp_root,
			      typename ItemArray<Node<T> >::ItemId p_z, ItemArray<Node<T> >& ia) {
		unsigned p_y = ItemArray<Node<T> >::Null;

		if (ia[p_z].left == ItemArray<Node<T> >::Null || ia[p_z].right == ItemArray<Node<T> >::Null) {
			p_y = p_z;
		} else {
			p_y = treeSuccessor(p_z, ia);
		}

		unsigned p_x = ItemArray<Node<T> >::Null;
		if (ia[p_y].left != ItemArray<Node<T> >::Null) {
			p_x = ia[p_y].left;
		} else {
			p_x = ia[p_y].right;
		}

		unsigned p_x_parent = ItemArray<Node<T> >::Null;
		if (p_x != ItemArray<Node<T> >::Null) {
			ia[p_x].parent = p_x_parent = ia[p_y].parent;
		} else {
			p_x_parent = ia[p_y].parent;
		}

		if (ia[p_y].parent == ItemArray<Node<T> >::Null) {
			*pp_root = p_x;
		} else {
			if (p_y == ia[ia[p_y].parent].left){
				ia[ia[p_y].parent].left = p_x;
			} else {
				ia[ia[p_y].parent].right = p_x;
			}
		}

		if (p_y != p_z) {
			ia[p_z].key = ia[p_y].key;
		}

		if (ia[p_y].color == BLACK) {
			rbDeleteFixup(pp_root, p_x, p_x_parent, ia);
		}
	}

	template <typename T>
	void rbDeleteFixup(typename ItemArray< Node<T> >::ItemId * root,
					   typename ItemArray< Node<T> >::ItemId x,
					   typename ItemArray< Node<T> >::ItemId x_parent,
					   ItemArray<Node<T> >& ia) {

		while (x != *root && color(x, ia) == BLACK) {
			if (x == ia[x_parent].left) {
				unsigned w = ItemArray<Node<T> >::Null;

				w = ia[x_parent].right;

				if (color(w, ia) == RED) {
					ia[w].color = BLACK;
					ia[x_parent].color = RED;
					leftRotate<T>(root, x_parent, ia);
					w = ia[x_parent].right;
				}

				if (color(ia[w].left, ia) == BLACK && color(ia[w].right, ia) == BLACK) {
					ia[w].color = RED;
					x = x_parent;
				} else {
					if (color(ia[w].right, ia) == BLACK) {
						ia[ia[w].left].color = BLACK;
						ia[w].color = RED;

						rightRotate<T>(root, w, ia);
						w = ia[x_parent].right;
					}

					ia[w].color = ia[x_parent].color;
					ia[x_parent].color = BLACK;
					ia[ia[w].right].color = BLACK;
					leftRotate<T>(root, x_parent, ia);
					x = *root;
				}
			} else {
				unsigned w = ItemArray<Node<T> >::Null;

				w = ia[x_parent].left;

				if (color(w, ia) == RED) {
					ia[w].color = BLACK;
					ia[x_parent].color = RED;
					rightRotate<T>(root, x_parent, ia);
					w = ia[x_parent].left;
				}

				if (color(ia[w].left, ia) == BLACK && color(ia[w].right, ia) == BLACK) {
					ia[w].color = RED;
					x = x_parent;
				} else {
					if (color(ia[w].right, ia) == BLACK) {
						ia[ia[w].left].color = BLACK;
						ia[w].color = RED;

						leftRotate<T>(root, w, ia);
						w = ia[x_parent].left;
					}

					ia[w].color = ia[x_parent].color;
					ia[x_parent].color = BLACK;
					ia[ia[w].left].color = BLACK;
					rightRotate<T>(root, x_parent, ia);
					x = *root;
				}
			}
		}

		if (x != ItemArray<Node<T> >::Null) {
			ia[x].color = BLACK;
		}
	}

	template <typename T>
	unsigned treeMinimum(typename ItemArray< Node<T> >::ItemId p_x, ItemArray<Node<T> >& ia) {
		while (ia[p_x].left != ItemArray<Node<T> >::Null) {
			p_x = ia[p_x].left;
		}

		return p_x;
	}

	template <typename T>
	unsigned treeSuccessor(unsigned p_x, ItemArray<Node<T> >& ia) {
		if (ia[p_x].right != ItemArray<Node<T> >::Null) {
			return treeMinimum(ia[p_x].right, ia);
		}

		unsigned p_y = ia[p_x].parent;

		while (p_y != ItemArray<Node<T> >::Null && p_x == ia[p_y].right) {
			p_x = p_y;
			p_y = ia[p_y].parent;
		}

		return p_y;
	}

	template <typename T>
	unsigned treeNewNode(const T& value, ItemArray<Node<T> >& ia) {
		return ia.place(Node<T>(value));
	}

	template <typename T>
	typename ItemArray<Node<T> >::ItemId treeInsert(
		unsigned * pp_root, const T& value,
		ItemArray<Node<T> >& ia) {

		if (*pp_root == ItemArray<Node<T> >::Null) {
			*pp_root = treeNewNode(value, ia);

			return *pp_root;
		}

		unsigned * pp_node = pp_root;
		unsigned p_parent = ItemArray<Node<T> >::Null;

		unsigned newNode = treeNewNode(value, ia);

		while (*pp_node != ItemArray<Node<T> >::Null) {
			if (ia[*pp_node].key == value) {
				return ItemArray<Node<T> >::Null;
			}

			p_parent = *pp_node;

			if (value < ia[(*pp_node)].key) {
				pp_node = &(ia[*pp_node].left);
			} else {
				pp_node = &(ia[*pp_node].right);
			}
		}

		*pp_node = newNode;
		ia[(*pp_node)].parent = p_parent;

		return *pp_node;
	}

	template <typename T>
	NodeColor color(typename ItemArray< Node<T> >::ItemId node, ItemArray<Node<T> >& ia) {
		if (node == ItemArray<Node<T> >::Null) {
			return BLACK;
		}

		return ia[node].color;
	}

	template <typename T>
	bool rbTreeInsert(typename ItemArray< Node<T> >::ItemId * pp_root, const T& key, ItemArray<Node<T> >& ia) {
		unsigned x = treeInsert(pp_root, key, ia);

		if (x == ItemArray<Node<T> >::Null) {
			return false;
		}

		ia[x].color = RED;

		while (x != *pp_root && ia[ia[x].parent].color == RED) {
			if (ia[x].parent == ia[ia[ia[x].parent].parent].left) {
				unsigned y = ia[ia[ia[x].parent].parent].right;

				if (color(y, ia) == RED) {
					ia[ia[x].parent].color = BLACK;
					ia[y].color = BLACK;
					ia[ia[ia[x].parent].parent].color = RED;

					x = ia[ia[x].parent].parent;
				} else if (x == ia[ia[x].parent].right) {
					x = ia[x].parent;
					leftRotate<T>(pp_root, x, ia);
				}

				if (ia[x].parent != ItemArray<Node<T> >::Null) {
					ia[ia[x].parent].color = BLACK;

					if (ia[ia[x].parent].parent != ItemArray<Node<T> >::Null) {
						ia[ia[ia[x].parent].parent].color = RED;

						rightRotate<T>(pp_root, ia[ia[x].parent].parent, ia);
					}
				}
			} else {
				unsigned y = ia[ia[ia[x].parent].parent].left;

				if (color(y, ia) == RED) {
					ia[ia[x].parent].color = BLACK;
					ia[y].color = BLACK;
					ia[ia[ia[x].parent].parent].color = RED;

					x = ia[ia[x].parent].parent;
				} else if (x == ia[ia[x].parent].left) {
					x = ia[x].parent;
					rightRotate<T>(pp_root, x, ia);
				}

				if (ia[x].parent != ItemArray<Node<T> >::Null) {
					ia[ia[x].parent].color = BLACK;

					if (ia[ia[x].parent].parent != ItemArray<Node<T> >::Null) {
						ia[ia[ia[x].parent].parent].color = RED;

						leftRotate<T>(pp_root, ia[ia[x].parent].parent, ia);
					}
				}	
			}
		}

		ia[*pp_root].color = BLACK;

		return true;
	}

	template <typename T>
	void destroy(typename ItemArray<T>::ItemId p_node, ItemArray<T> & ia) {
		if (p_node != ItemArray<T>::Null) {
			if (ia[p_node].left != ItemArray<T>::Null) {
				destroy<T>(ia[p_node].left, ia);
			}

			if (ia[p_node].right != ItemArray<T>::Null) {
				destroy<T>(ia[p_node].right, ia);
			}
		}
	}

	template<typename T>
	void Node<T>::serialize(ostream& os, int tabs, ItemArray<Node<T> >& ia) {
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

		if (left == ItemArray<Node<T> >::Null) {
			os << "null";
		} else {
			ia[left].serialize(os, tabs, ia);
		}

		os << ",";

		os << newline;
		os << "right: ";

		if (right == ItemArray<Node<T> >::Null) {
			os << "null";
		} else {
			ia[right].serialize(os, tabs, ia);
		}

		--tabs;
		os << newline;
		os << "}";
	}

	template <typename T>
	std::ostream& operator<<(std::ostream& os, Node<T>& node) {
		os << "[key: " << node.key << ", left: " << node.left << ", right: " << node.right << ", parent: " << node.parent << "]";
		return os;
	}
}
