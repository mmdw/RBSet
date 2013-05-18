#pragma once

#include <memory>
#include <iostream>

#include "RBSetException.h"

#include "AbstractSet.h"
#include "RBTree/RBNodeParser.h"
#include "RBTree/RBNode.h"
#include "Contract/RBSetContractChecker.h"
#include "RBSetIterator.h"

template <typename T>
class RBSet : public AbstractSet<T> {
	friend class RBSetContractChecker<T>;

public:
	RBSet();
	RBSet(istream& os);
	RBSet(const RBSet& rhs);

	~RBSet();

	void serialize(ostream& os) const;
	void remove(const T& value);
	void put(const T&);
	size_t size() const;
	bool contains(const T& value) const;

	AbstractIterator<T> * const iterator();

	void debug(std::ostream& os);
	void checkInvariant() const;

	RBSet& operator>>(T& rhs);
	RBSet& operator<<(const T& rhs);

private:
	RBSetContractChecker<T> contractChecker;
	typedef Tree::Node<T> TreeNode;

	ItemArray<TreeNode> ia;
	typename ItemArray<TreeNode>::ItemId p_root;
	size_t count;
};



template<typename T>
RBSet<T>::RBSet() : contractChecker(*this), p_root(ItemArray<TreeNode>::Null), count(0) {
	checkInvariant();
}

template<typename T>
RBSet<T>::RBSet(std::istream& is) : contractChecker(*this), p_root(ItemArray<TreeNode>::Null), count(0) {
	Tree::RBNodeParser parser(is);
	p_root = parser.parseRbNode(count, ia);

	checkInvariant();
}

template<typename T>
RBSet<T>::RBSet(const RBSet& rhs) : contractChecker(*this), count(rhs.count) {
	p_root = copyNode(rhs.p_root, rhs.ia, ia);

	checkInvariant();
}

template<typename T>
RBSet<T>::~RBSet() {
	if (p_root != ItemArray<TreeNode>::Null) {
		Tree::destroy<TreeNode>(p_root, ia);
	}
}

template<typename T>
void RBSet<T>::put(const T& value) {
	checkInvariant();

	if (rbTreeInsert(&p_root, value, ia)) {
		count++;
	}

	checkInvariant();
	contractChecker.checkPutPostcondition(value);
}

template<typename T>
void RBSet<T>::serialize(ostream& os) const {
	checkInvariant();

	if (p_root == ItemArray<TreeNode>::Null) {
		os << "null";
	} else {
		ia[p_root].serialize(os, 0, ia);
	}

	checkInvariant();
}

template<typename T>
void RBSet<T>::remove(const T& value) {
	checkInvariant();

	typename ItemArray<TreeNode>::ItemId p_node = p_root;

	while (p_node != ItemArray<TreeNode>::Null && ia[p_node].key != value) {
		p_node = value < ia[p_node].key ? ia[p_node].left : ia[p_node].right;
	}

	if (p_node != ItemArray<TreeNode>::Null) {
		rbDelete(&p_root, p_node, ia);
		--count;
	} else {
		throw RBSetException("RBSet::remove : set does not contain item");
	}

	checkInvariant();
	contractChecker.checkRemovePostcondition(value);
}

template<typename T>
size_t RBSet<T>::size() const {
	checkInvariant();

	return count;
}

template<typename T>
AbstractIterator<T>* const RBSet<T>::iterator() {
	typename ItemArray<TreeNode>::ItemId p_node = p_root;
	if (p_node != ItemArray<TreeNode>::Null) {
		while (ia[p_node].left != ItemArray<TreeNode>::Null) {
			p_node = ia[p_node].left;
		}
	}

	return new RBSetIterator<T>(p_node, p_root, ia);
}

template<typename T>
bool RBSet<T>::contains(const T& value) const {
	checkInvariant();
	typename ItemArray<TreeNode>::ItemId p_node = p_root;

	while (p_node != ItemArray<TreeNode>::Null) {
		if (value == ia[p_node].key) {
			return true;
		} else if (value < ia[p_node].key) {
			p_node = ia[p_node].left;
		} else {
			p_node = ia[p_node].right;
		}
	}

	return false;
}


template <typename T>
void RBSet<T>::debug(std::ostream& os) {
	ia.debug(os);
}

template <typename T>
void RBSet<T>::checkInvariant() const {
	contractChecker.checkInvariant();
}

template <typename T>
RBSet<T>& RBSet<T>::operator>>(T& rhs) {
	checkInvariant();

	typename ItemArray<TreeNode>::ItemId p_node = p_root;
	if (p_node != ItemArray<TreeNode>::Null) {
		while (ia[p_node].left != ItemArray<TreeNode>::Null) {
			p_node = ia[p_node].left;
		}
	}

	if (p_node == ItemArray<TreeNode>::Null) {
		throw RBSetException("RBSet::operator>> : set is empty");
	}

	rhs = ia[p_node].key;
	remove(rhs);

	return *this;
}

template <typename T>
RBSet<T>& RBSet<T>::operator<<(const T& rhs) {
	put(rhs);

	return *this;
}
