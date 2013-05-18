#pragma once

#include <list>

template <typename T>
class RBSet;

/**
 * Проверяет соответствие состояния объекта RBSet контрактам.
 */
template <typename T>
class RBSetContractChecker {
public:
	/**
	 * Конструктор
	 * @param set ссылка на множество.
	 */
	RBSetContractChecker(RBSet<T>& set);

	/**
	 * Выполняет проверку инварианта
	 */
	void checkInvariant() const;

	/**
	 * Проверка того, что элемент добавлен.
	 * @param value добавленный элкмент
	 */
	void checkPutPostcondition(const T& value) const;

	/**
	 * Проверка на то, что элемент удален.
	 * @param value удаленный элемент
	 */
	void checkRemovePostcondition(const T& value) const;

private:
	/**
	 * Ссылка на множество
	 */
	RBSet<T>& set;

	/**
	 * Проверка корректности ссылок на родительский узел.
	 */
	void checkIntegrity(typename ItemArray<Tree::Node<T> >::ItemId node) const;

	/**
	 * Свойство 3
	 *
	 * Если вершина красная, оба ее ребенка -- черные.
	 */
	void checkProperty3(typename ItemArray<Tree::Node<T> >::ItemId node) const;

	/**
	 * Свойство 4
	 *
	 * Все пути, идущие вниз от корня к листьям, содержат одинаковое количество черных вершин.
	 */
	void checkProperty4() const;

	/**
	 * Выполняет подсчет черных вершин по всем путям.
	 */
	std::list<unsigned> countBlack(typename ItemArray<Tree::Node<T> >::ItemId root) const;
};

template <typename T>
RBSetContractChecker<T>::RBSetContractChecker(RBSet<T>& set) : set(set) {

}

template <typename T>
void RBSetContractChecker<T>::checkInvariant() const {
	checkIntegrity(set.p_root);
	checkProperty3(set.p_root);
	checkProperty4();
}

template <typename T>
void RBSetContractChecker<T>::checkProperty3(typename ItemArray<Tree::Node<T> >::ItemId node) const {
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
void RBSetContractChecker<T>::checkProperty4() const {
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
		typename ItemArray<Tree::Node<T> >::ItemId root) const {

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

template<typename T>
inline void RBSetContractChecker<T>::checkIntegrity(
		typename ItemArray<Tree::Node<T> >::ItemId node) const {

	if (node != ItemArray<Tree::Node<T> >::Null) {
		if (set.ia[node].left != ItemArray<Tree::Node<T> >::Null) {
			assert(node == set.ia[set.ia[node].left].parent);

			checkIntegrity(set.ia[node].left);
		}

		if (set.ia[node].right != ItemArray<Tree::Node<T> >::Null) {
			assert(node == set.ia[set.ia[node].right].parent);

			checkIntegrity(set.ia[node].right);
		}
	}
}

template<typename T>
void RBSetContractChecker<T>::checkPutPostcondition(const T& value) const {
	assert(set.contains(value));
}

template<typename T>
void RBSetContractChecker<T>::checkRemovePostcondition(const T& value) const {
	assert(!set.contains(value));
}
