#pragma once

#include <memory>
#include <iostream>

#include "AbstractSet.h"
#include "RBTree/RBNodeParser.h"
#include "RBTree/RBNode.h"

template <typename T>
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

	void debug(std::ostream& os);

private:
	typedef Tree::Node<T> TreeNode;
	
	ItemArray<TreeNode> ia;
	typename ItemArray<TreeNode>::ItemId p_root;
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
		typename ItemArray<TreeNode>::ItemId p_node;
		typename ItemArray<TreeNode>::ItemId p_root;
		ItemArray<TreeNode>& ia;

		RBSetIterator(typename ItemArray<TreeNode>::ItemId p_node,
				      typename ItemArray<TreeNode>::ItemId p_root,
				      ItemArray<TreeNode>& ia);
	};
};

template<typename T>
RBSet<T>::RBSet() : p_root(ItemArray<TreeNode>::null), count(0) {

}

template<typename T>
RBSet<T>::RBSet(std::istream& is) : p_root(ItemArray<TreeNode>::null), count(0) {
	Tree::RBNodeParser parser(is);

	p_root = parser.parseRbNode(count, ia);
}

template<typename T>
RBSet<T>::RBSet(const RBSet& rhs) : count(rhs.count) {
	p_root = copyNode(rhs.p_root, ia);
}

template<typename T>
RBSet<T>::~RBSet() {
	if (p_root != ItemArray<TreeNode>::null) {
		Tree::destroy<TreeNode>(p_root, ia);
	}
}

template<typename T>
void RBSet<T>::put(const T& value) {
	if (rbTreeInsert(&p_root, value, ia)) {
		count++;
	}
}

template<typename T>
void RBSet<T>::serialize(ostream& os) {
	if (p_root == ItemArray<TreeNode>::null) {
		os << "null";
	} else {
		ia[p_root].serialize(os, 0, ia);
	}
}

template<typename T>
void RBSet<T>::remove(const T& value) {
	typename ItemArray<TreeNode>::ItemId p_node = p_root;

	while (p_node != ItemArray<TreeNode>::null && ia[p_node].key != value) {
		p_node = ia[p_node].key > value ? ia[p_node].left : ia[p_node].right;
	}

	if (p_node != ItemArray<TreeNode>::null) {
		rbDelete(&p_root, p_node, ia);
		--count;
	}
}

template<typename T>
size_t RBSet<T>::size() const {
	return count;
}

template<typename T>
AbstractIterator<T>* RBSet<T>::iterator() {
	typename ItemArray<TreeNode>::ItemId p_node = p_root;
	if (p_node != ItemArray<TreeNode>::null) {
		while (ia[p_node].left != ItemArray<TreeNode>::null) {
			p_node = ia[p_node].left;
		}
	}

	return new RBSetIterator(p_node, p_root, ia);
}

template<typename T>
bool RBSet<T>::contains(const T& value) const {
	return false;
}

template<typename T>
RBSet<T>::RBSetIterator::RBSetIterator(typename ItemArray<TreeNode>::ItemId p_node,
									   typename ItemArray<TreeNode>::ItemId p_root,
									   ItemArray<TreeNode>& ia)
	: p_node(p_node), p_root(p_root), ia(ia) {

}

template<typename T>
void RBSet<T>::RBSetIterator::next() {
	if (p_node == ItemArray<TreeNode>::null) {
		std::cerr << "end reached";
		exit(-1);
	}

	if (ia[p_node].right != ItemArray<TreeNode>::null) {
		p_node = ia[p_node].right;

		while (ia[p_node].left != ItemArray<TreeNode>::null) {
			p_node = ia[p_node].left;
		}
	} else {
		while (ia[p_node].parent != ItemArray<TreeNode>::null && p_node == ia[ia[p_node].parent].right) {
			p_node = ia[p_node].parent;
		}

		p_node = ia[p_node].parent;
	}
}

template<typename T>
void RBSet<T>::RBSetIterator::prev() {
	if (p_node == ItemArray<TreeNode>::null && p_root == ItemArray<TreeNode>::null) {
		std::cerr << "empty tree";
		exit(-1);
	}

	if (p_node == ItemArray<TreeNode>::null) {
		p_node = p_root;
		while (ia[p_node].right != ItemArray<TreeNode>::null) {
			p_node = ia[p_node].right;
		}
	} else {
		if (ia[p_node].left != ItemArray<TreeNode>::null) {
			p_node = ia[p_node].left;

			while (ia[p_node].right != ItemArray<TreeNode>::null) {
				p_node = ia[p_node].right;
			}
		} else {
			while (ia[p_node].parent != ItemArray<TreeNode>::null && p_node == ia[ia[p_node].parent].left) {
				p_node = ia[p_node].parent;
			}

			p_node = ia[p_node].parent;
		}
	}
}

template<typename T>
bool RBSet<T>::RBSetIterator::hasNext() {
	return p_node != ItemArray<TreeNode>::null;
}

template<typename T>
T& RBSet<T>::RBSetIterator::item() {
	return ia[p_node].key;
}

template <typename T>
void RBSet<T>::debug(std::ostream& os) {
	ia.debug(os);
}
