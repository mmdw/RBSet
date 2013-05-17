#pragma once

#include <istream>
#include <ostream>
#include <cassert>

/**
 * Пример вектора, с которым может работать контейнер
 */
struct Vector2D {
	float a, b;

	/**
	 * Должен быть определен конструктор по умолчанию.
	 */
	Vector2D() : a(0), b(0) {

	}

	Vector2D(float a, float b) :a(a), b(b) {

	}

	/**
	 * Должны быть определены операторы < !=, ==
	 */
	bool operator<(const Vector2D& rhs) const {
		return a < rhs.a ? true : b < rhs.b;
	}

	bool operator!=(const Vector2D& rhs) const {
		return a != rhs.a || b != rhs.b;
	}

	bool operator==(const Vector2D& rhs) const {
		return a == rhs.a && b == rhs.b;
	}
};

/**
 * Вывод в поток. Для сериализации.
 *
 * Нельзя использовать символ ','.
 */
std::ostream&  operator<<(std::ostream& os, const Vector2D& v) {
	os << '(' << v.a << "; " << v.b << ')';
	return os;
}

/**
 * Чтение из потока. 
 */
std::istream& operator>>(std::istream& is,  Vector2D& v) {
	assert(is.get() == '(');
	is >> v.a;

	assert(is.get() == ';');
	is >> v.b;

	assert(is.get() == ')');

	return is;
}