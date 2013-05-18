#pragma once

/**
 * Интерфейс итератора
 */
template <typename T>
class AbstractIterator {
public:
	/**
	 * Переход к следующему элементу
	 */
	virtual void next() = 0;

	/**
	 * Переход к предыдущему элементу
	 */
	virtual void prev() = 0;

	/**
	 * Проверка наличия следующего элемента
	 *
	 * @return true, если есть
	 */
	virtual bool hasNext() const = 0;

	/**
	 * Получение ссылки на элемент
	 *
	 * @return ссылка на элемент
	 */
	virtual T& item() = 0;

	/**
	 * Деструктор
	 */
	virtual ~AbstractIterator() { }

protected:
	/**
	 * Конструктор
	 */
	AbstractIterator() { }
};
