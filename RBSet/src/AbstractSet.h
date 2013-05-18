#pragma once
#include "AbstractIterator.h"

/**
 * Интерфейс множества
 */
template <typename T>
class AbstractSet {
public:
	/**
	 * Помещает копию элемена в множество
	 */
	virtual void put(const T& value) = 0;

	/**
	 * Возвращает количество элементов в множестве
	 * @return количество элементов
	 */
	virtual size_t size() const = 0;

	/**
	 * Удаляет элемент из множества
	 * @param value значение
	 */
	virtual void remove(const T& value) = 0;

	/**
	 * Возвращает итератор
	 *
	 * @return итератор, указывающий на минимальный элемент
	 */
	virtual AbstractIterator<T> * const iterator() = 0;

	/**
	 * Проверяет наличие значения в множестве
	 *
	 * @param value значение
	 */
	virtual	bool contains(const T& value) const = 0;

	virtual ~AbstractSet() { }
private:
};
