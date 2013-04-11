#pragma once

#include "AbstractSet.h"
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

private:
	Tree::Node<T> * p_root;
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

template<typename T>
RBSet<T>::RBSet() : p_root(NULL), count(0) {

}

template<typename T>
RBSet<T>::RBSet(std::istream& is) : p_root(NULL), count(0) {
	parseRbNode(is, &p_root, count);
}

template<typename T>
RBSet<T>::RBSet(const RBSet& rhs) : count(rhs.count) {
	p_root = copyNode(rhs.p_root);
}

template<typename T>
RBSet<T>::~RBSet() {

}

template<typename T>
void RBSet<T>::put(const T& value) {
	if (rbTreeInsert(&p_root, value)) {
		count++;
	}
}

template<typename T>
void RBSet<T>::serialize(ostream& os) {
	if (p_root == NULL) {
		os << "null";
	} else {
		p_root->serialize(os, 0);
	}
}

template<typename T>
void RBSet<T>::remove(const T& value) {
	Tree::Node<T>* p_node = p_root;

	while (p_node != NULL && p_node->key != value) {
		p_node = p_node->key > value ? p_node->left : p_node->right;
	}

	if (p_node != NULL) {
		rbDelete(&p_root, p_node);
		--count;
	}
}

template<typename T>
size_t RBSet<T>::size() const {
	return count;
}

template<typename T>
AbstractIterator<T>* RBSet<T>::iterator() {
	Tree::Node<T>* p_node = p_root;
	if (p_node != NULL) {
		while (p_node->left != NULL) {
			p_node = p_node->left;
		}
	}

	return new RBSetIterator(p_node, p_root);
}

template<typename T>
bool RBSet<T>::contains(const T& value) const {
	return false;
}

template<typename T>
RBSet<T>::RBSetIterator::RBSetIterator(Tree::Node<T>* p_node, Tree::Node<T>* p_root)
	: p_node(p_node), p_root(p_root) {

}

template<typename T>
void RBSet<T>::RBSetIterator::next() {
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

template<typename T>
void RBSet<T>::RBSetIterator::prev() {
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

template<typename T>
bool RBSet<T>::RBSetIterator::hasNext() {
	return p_node != NULL;
}

template<typename T>
T& RBSet<T>::RBSetIterator::item() {
	return p_node->key;
}
