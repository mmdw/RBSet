/*
 * RBTree.h
 *
 *  Created on: 03.03.2013
 *      Author: user
 */

#pragma once

#include <ostream>
#include "RBNodeParser.h"
#include "ItemArray.h"

using std::ostream;
using std::istream;
using std::endl;

namespace Tree {
	template <typename T>
	class RBTree {
	protected:
		size_t count;

		ItemArray<Node<T> > ia;
		ItemArray<Node<T> >::ItemId p_root;

	public:
		RBTree() : p_root(ItemArray<Node<T> >::null), count(0) {
		}

		RBTree(const RBTree& rhs) : count (rhs.count) {
			p_root = copyNode(rhs.p_root, ia);
		}

		RBTree(istream& os);

		void put(const T& key);
		void serialize(ostream& os);
		size_t size() const;

		virtual ~RBTree() {
			delete p_root;
		}

		void erase(const T& key);
	};

	template<typename T>
	RBTree<T>::RBTree(istream& is) : p_root(ItemArray<Node<T> >::null), count(0) {
		RBNodeParser(is).parseRbNode(&p_root, count, ia);
	}

	template<typename T>
	void RBTree<T>::serialize(ostream& os) {
		if (p_root == ItemArray<Node<T> >::null) {
			os << "null";
		} else {
			ia[p_root].serialize(os, 0, ia);
		}
	}

	template <typename T>
	size_t RBTree<T>::size() const {
		return count;
	}

	template <typename T>
	void RBTree<T>::put(const T& key) {
		if (rbTreeInsert(&p_root, key, ia)) {
			count++;
		}
	}

	template <typename T>
	void RBTree<T>::erase(const T& key) {
		ItemArray<Node<T> >::ItemId p_node = p_root;

		while (p_node != ItemArray<Node<T> >::null && ia[p_node].key != key) {
			p_node = ia[p_node].key > key ? ia[p_node].left : ia[p_node].right;
		}

		if (p_node != NULL) {
			rbDelete(&p_root, p_node, ia);
			--count;
		}
	}
}
