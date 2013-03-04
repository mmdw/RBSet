#include <cstdlib>
#include <iostream>

#include "parserUtil.h"

using std::istream;

void error(const char* msg) {
	std::cerr << msg;

	exit(-1);
}

char next(istream& is) {
	char ch;

	is.get(ch);
	is.unget();

	return ch;
}

char read(istream& is) {
	return is.get();
}

bool hasNext(istream& is) {
	return next(is) != '\0';
}

void skipWs(istream& is) {
	char ch;
	is.get(ch);

	while (isspace(ch)) {
		is.get(ch);
	}

	is.unget();
}

string readWord(istream& is) {
	string result;

	while (isalpha(next(is))) {
		result += read(is);
	}

	return result;
}

string readNumber(istream& is) {
	string result;

	while (isdigit(next(is))) {
		result += read(is);
	}

	return result;
}
