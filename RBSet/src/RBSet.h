#pragma once

#include <memory>

#include <iostream>

#include "AbstractSet.h"
#include "RBTree/RBNode.h"

template <typename T, class Allocator = std::allocator<T> >
class RBSet : public AbstractSet<T> {
public:
	RBSet();
	RBSet(istream& os);
	RBSet(const RBSet& rhs);

	void serialize(ostream& os);
	void remove	(const T& value);
	void put(const T&);
	size_t size() const;
	bool contains(const T& value) const;

	AbstractIterator<T>* iterator();

	~RBSet();

private:
	typedef Tree::Node<T> TreeNode;

	Allocator itemAlloc;
	typename TreeNode::NodeAllocator nodeAlloc;
	
	typename TreeNode::NodeAllocator::pointer p_root;
	size_t count;

	class RBSetIterator : public AbstractIterator<T> {
	public:
		friend AbstractIterator<T>* RBSet::iterator();

		RBSetIterator(const RBSetIterator& rhs);

		void next();
		void prev();
		bool hasNext();
		T& item();

	private:
		Tree::Node<T>* p_node;
		Tree::Node<T>* p_root;

		RBSetIterator(Tree::Node<T>* p_node, Tree::Node<T>* p_root);
	};
};

template<typename T, class A>
RBSet<T, A>::RBSet() : p_root(NULL), count(0) {

}

template<typename T, typename A>
RBSet<T, A>::RBSet(std::istream& is) : p_root(NULL), count(0) {
	parseRbNode(is, &p_root, count);
}

template<typename T, typename A>
RBSet<T, A>::RBSet(const RBSet& rhs) : count(rhs.count) {
	p_root = copyNode(rhs.p_root, nodeAlloc, itemAlloc);
}

template<typename T, typename A>
RBSet<T, A>::~RBSet() {
	if (p_root != NULL) {
		Tree::destroy<T>(p_root, nodeAlloc, itemAlloc);
	}
}

template<typename T, typename A>
void RBSet<T, A>::put(const T& value) {
	if (rbTreeInsert(&p_root, value, nodeAlloc, itemAlloc)) {
		count++;
	}
}

template<typename T, typename A>
void RBSet<T, A>::serialize(ostream& os) {
	if (p_root == qNULL) {
		os << "null";
	} else {
		p_root->serialize(os, 0);
	}
}

template<typename T, typename A>
void RBSet<T, A>::remove(const T& value) {
	Tree::Node<T>* p_node = p_root;

	while (p_node != NULL && p_node->key != value) {
		p_node = p_node->key > value ? p_node->left : p_node->right;
	}

	if (p_node != NULL) {
		rbDelete(&p_root, p_node);
		--count;
	}
}

template<typename T, typename A>
size_t RBSet<T, A>::size() const {
	return count;
}

template<typename T, typename A>
AbstractIterator<T>* RBSet<T, A>::iterator() {
	Tree::Node<T>* p_node = p_root;
	if (p_node != NULL) {
		while (p_node->left != NULL) {
			p_node = p_node->left;
		}
	}

	return new RBSetIterator(p_node, p_root);
}

template<typename T, typename A>
bool RBSet<T, A>::contains(const T& value) const {
	return false;
}

template<typename T, typename A>
RBSet<T, A>::RBSetIterator::RBSetIterator(TreeNode * p_node, TreeNode* p_root)
	: p_node(p_node), p_root(p_root) {

}

template<typename T, typename A>
void RBSet<T, A>::RBSetIterator::next() {
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
}

template<typename T, typename A>
void RBSet<T, A>::RBSetIterator::prev() {
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
}

template<typename T, typename A>
bool RBSet<T, A>::RBSetIterator::hasNext() {
	return p_node != NULL;
}

template<typename T, typename A>
T& RBSet<T, A>::RBSetIterator::item() {
	return p_node->key;
}
