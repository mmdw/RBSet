/*
 * RBSetContrat.h
 *
 *  Created on: 17.05.2013
 *      Author: user
 */
#pragma once

#include <list>

template <typename T>
class RBSet;

template <typename T>
class RBSetContractChecker {
public:
	RBSetContractChecker(RBSet<T>& set);

	/**
	 * Выполняет проверку инварианта
	 */
	void checkInvariant();
private:
	RBSet<T>& set;

	/**
	 * Свойство 3
	 *
	 * Если вершина красная, оба ее ребенка -- черные.
	 */
	void checkProperty3(typename ItemArray<Tree::Node<T> >::ItemId node);

	/**
	 * Свойство 4
	 *
	 * Все пути, идущие вниз от корня к листьям, содержат одинаковое количество черных вершин.
	 */
	void checkProperty4();

	/**
	 * Выполняет подсчет черных вершин по всем путям.
	 */
	std::list<unsigned> countBlack(typename ItemArray<Tree::Node<T> >::ItemId root);
};

template <typename T>
RBSetContractChecker<T>::RBSetContractChecker(RBSet<T>& set) : set(set) {

}

template <typename T>
void RBSetContractChecker<T>::checkInvariant() {
	checkProperty3(set.p_root);
	checkProperty4();
}

template <typename T>
void RBSetContractChecker<T>::checkProperty3(typename ItemArray<Tree::Node<T> >::ItemId node) {
	if (node != ItemArray<Tree::Node<T> >::Null) {
		if (color(node, set.ia) == Tree::RED) {
			assert(Tree::color(set.ia[node].left,  set.ia) == Tree::BLACK);
			assert(Tree::color(set.ia[node].right, set.ia) == Tree::BLACK);
		}

		checkProperty3(set.ia[node].left);
		checkProperty3(set.ia[node].right);
	}
}

template <typename T>
void RBSetContractChecker<T>::checkProperty4() {
	std::list<unsigned> paths = countBlack(set.p_root);
	std::list<unsigned>::iterator it = paths.begin();

	unsigned firstCount = *it;
	++it;

	for (/* */; it != paths.end(); ++it) {
		assert(firstCount == *it);
	}
}

template<typename T>
inline std::list<unsigned> RBSetContractChecker<T>::countBlack(
		typename ItemArray<Tree::Node<T> >::ItemId root) {

	std::list<unsigned> result;

	if (root != ItemArray<Tree::Node<T> >::Null) {
		std::list<unsigned> childResult = countBlack(set.ia[root].left);
		result.splice(result.end(), childResult);

		childResult = countBlack(set.ia[root].right);
		result.splice(result.end(), childResult);

		std::list<unsigned>::iterator it = result.begin();
		if (Tree::color(root, set.ia) == Tree::BLACK) {
			for (/* 3 */; it != result.end(); ++it) {
				++(*it);
			}
		}

	} else {
		result.push_back(1);
	}

	return result;
}
