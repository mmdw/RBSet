#pragma once

#include <istream>
#include <ostream>
#include <cassert>

/**
 * ������ �������, � ������� ����� �������� ���������
 */
struct Vector2D {
	float a, b;

	/**
	 * ������ ���� ��������� ����������� �� ���������.
	 */
	Vector2D() : a(0), b(0) {

	}

	Vector2D(float a, float b) :a(a), b(b) {

	}

	/**
	 * ������ ���� ���������� ��������� < !=, ==
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
 * ����� � �����. ��� ������������.
 *
 * ������ ������������ ������ ','.
 */
std::ostream&  operator<<(std::ostream& os, const Vector2D& v) {
	os << '(' << v.a << "; " << v.b << ')';
	return os;
}

/**
 * ������ �� ������. 
 */
std::istream& operator>>(std::istream& is,  Vector2D& v) {
	assert(is.get() == '(');
	is >> v.a;

	assert(is.get() == ';');
	is >> v.b;

	assert(is.get() == ')');

	return is;
}