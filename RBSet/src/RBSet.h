/*! \mainpage RBSet
 * Множество на массиве с использованием красно-черного дерева.
 *
 * \section desc_sec Базовые операции
 * \subsection put добавление элементов
 * Можно выполнить с помощью метода @ref RBSet<T>::put(const T& value) и с помощью оператора <b>>></b>
 * \code
	RBSet<int> set;
	set.put(3);
	set.put(2);
	set << 1 << 4;
 * \endcode
 * \subsection get удаление элементов
 * выполняется с помощью метода @ref RBSet<T>::remove(const T& value)
 * \code
	RBSet<int> set;
	set.put(3);
	set.put(2);

	set.remove(3);
	set.remove(0); // будет брошено исключение
 * \endcode
 * \subsection iterator перебор элементов множества
 * Перечисление элементов выполняется с помошью итератора.
 * \code
	RBSet<int> set;
	set << 1 << 2 << 3 << 4;

	AbstractIterator<int> * const it = set.iterator();
	while (it->hasNext()) {
		std::cout << it->item() << ' ';

		it->next();
	}

	delete it;
 * \endcode
 * \subsection serialize сериализация
 * Множество может быть преобразовано в строку байт и восстановлено обратно. Строка байт предстваляет собой простой JSON-подобный формат.
 * <br/> Сохраняются элементы множества и структура красно-черного дерева.
 *
 * Пример:
 * \code
	RBSet<int> A;
	set << 3 << 2 << 1 << 4;

	std::ostringstream oss;
	A.serialize(oss);
 * \endcode
 * В поток <b>oss</b> будет записана следующая информация:
 * \code
	{
	  key: 2,
	  color: black,
	  left: {
	  	  key: 1,
	  	  color: black
	  },
	  right: {
	  	  key: 3,
	  	  color: black,
	  	  right: {
	  	  	  key: 4,
	  	  	  color: red
	  	  }
	  }
	}
 * \endcode
 * Чтобы восстановить объект, нужно в конструктор множества передать поток ввода:
 * \code
	std::istringstream iss(oss.str());
	RBSet<Vector2D> B(iss);
 * \endcode
 * Множество <b>B</b> будет содержать те же элементы, что и множество <b>А</b>.
 */

#pragma once

#include <memory>
#include <iostream>

#include "RBSetException.h"

#include "AbstractSet.h"
#include "RBTree/RBNodeParser.h"
#include "RBTree/RBNode.h"
#include "Contract/RBSetContractChecker.h"
#include "RBSetIterator.h"

/**
 * Множество на красно-черном дереве
 */
template <typename T>
class RBSet : public AbstractSet<T> {
	friend class RBSetContractChecker<T>;

public:
	RBSet();
	/**
	 * Конструирует множество из потока (десереализация).
	 */
	RBSet(istream& os);

	/**
	 * Конструктор копирования
	 *
	 * @param rhs оригинал
	 */
	RBSet(const RBSet& rhs);

	~RBSet();

	/**
	 * Выполняет сериализацию множества
	 *
	 * @param os поток
	 */
	void serialize(ostream& os) const;

	void remove(const T& value);
	void put(const T&);
	size_t size() const;
	bool contains(const T& value) const;

	AbstractIterator<T> * const iterator();

	/**
	 * Вывод отладочной информации
	 */
	void debug(std::ostream& os);

	/**
	 * Выполняет проверку инварианта
	 */
	void checkInvariant() const;

	/**
	 * Добавление элемента в множество
	 * @param rhs элемент
	 * @return ссылка на множество
	 */
	RBSet& operator>>(T& rhs);

	/**
	 * Извлечение элемента из множества.
	 * Элементы извлекаются в порядке возрастания.
	 *
	 * @param rhs элемент
	 * @return ссылка на множество
	 */
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
