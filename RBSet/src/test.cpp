/*
 * test.cpp
 *
 *  Created on: 15.05.2013
 *      Author: user
 */
#include "RBSet.h"

#include <iostream>
#include <string>
#include <sstream>

struct Vector2D {
	float a, b;

	Vector2D() : a(0), b(0) {

	}

	Vector2D(float a, float b) :a(a), b(b) {

	}

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

std::ostream&  operator<<(std::ostream& os, const Vector2D& v) {
	os << '(' << v.a << "; " << v.b << ')';
	return os;
}

std::istream& operator>>(std::istream& is,  Vector2D& v) {
	assert(is.get() == '(');
	is >> v.a;

	assert(is.get() == ';');
	is >> v.b;

	assert(is.get() == ')');

	return is;
}

int main(int argc, char ** argv) {
	RBSet<Vector2D> set;
	set.put(Vector2D(0, 0));
	set.put(Vector2D(0, 1));
	set.put(Vector2D(0, 2));
	set.put(Vector2D(0, 5));

	set.contains(Vector2D(0, 5));
	set.serialize(std::cout);

	return 0;
}

