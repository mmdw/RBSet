/*
 * RBSetIterator.h
 *
 *  Created on: 18.05.2013
 *      Author: user
 */
#pragma once


/**
 * Итератор для множества RBSet.
 */
template<typename T>
class RBSetIterator : public AbstractIterator<T> {
public:
	/**
	 * Дружественный метод класса RBSet, который создает новый итератор.
	 */
	friend AbstractIterator<T>* const RBSet<T>::iterator();

	/**
	 * Конструктор копирования
	 */
	RBSetIterator(const RBSetIterator& rhs);

	void next();
	void prev();
	bool hasNext() const;
	T& item();

private:
	typedef Tree::Node<T> TreeNode;
	typename ItemArray<TreeNode>::ItemId p_node;
	typename ItemArray<TreeNode>::ItemId p_root;
	ItemArray<TreeNode>& ia;

	RBSetIterator(typename ItemArray<TreeNode>::ItemId p_node,
			      typename ItemArray<TreeNode>::ItemId p_root,
			      ItemArray<TreeNode>& ia);
};


template<typename T>
RBSetIterator<T>::RBSetIterator(typename ItemArray<TreeNode>::ItemId p_node,
									   typename ItemArray<TreeNode>::ItemId p_root,
									   ItemArray<TreeNode>& ia)
	: p_node(p_node), p_root(p_root), ia(ia) {

}

template<typename T>
void RBSetIterator<T>::next() {
	if (p_node == ItemArray<TreeNode>::Null) {
		throw RBSetIteratorException("RBSet::RBSetIterator : end reached");
	}

	if (ia[p_node].right != ItemArray<TreeNode>::Null) {
		p_node = ia[p_node].right;

		while (ia[p_node].left != ItemArray<TreeNode>::Null) {
			p_node = ia[p_node].left;
		}
	} else {
		while (ia[p_node].parent != ItemArray<TreeNode>::Null && p_node == ia[ia[p_node].parent].right) {
			p_node = ia[p_node].parent;
		}

		p_node = ia[p_node].parent;
	}
}

template<typename T>
void RBSetIterator<T>::prev() {
	if (p_node == ItemArray<TreeNode>::Null && p_root == ItemArray<TreeNode>::Null) {
		throw RBSetIteratorException("RBSet::RBSetIterator : tree is empty");
	}

	if (p_node == ItemArray<TreeNode>::Null) {
		p_node = p_root;
		while (ia[p_node].right != ItemArray<TreeNode>::Null) {
			p_node = ia[p_node].right;
		}
	} else {
		if (ia[p_node].left != ItemArray<TreeNode>::Null) {
			p_node = ia[p_node].left;

			while (ia[p_node].right != ItemArray<TreeNode>::Null) {
				p_node = ia[p_node].right;
			}
		} else {
			while (ia[p_node].parent != ItemArray<TreeNode>::Null && p_node == ia[ia[p_node].parent].left) {
				p_node = ia[p_node].parent;
			}

			p_node = ia[p_node].parent;
		}
	}
}

template<typename T>
bool RBSetIterator<T>::hasNext() const {
	return p_node != ItemArray<TreeNode>::Null;
}

template<typename T>
T& RBSetIterator<T>::item() {
	return ia[p_node].key;
}
