#include <cstdlib>
#include <iostream>

#include "parserUtil.h"

using std::istream;
namespace Tree {
	void ParserUtil::error(const char* msg) {
		std::cerr << msg;

		exit(-1);
	}

	char ParserUtil::next(istream& is) {
		char ch;

		is.get(ch);
		is.unget();

		return ch;
	}

	char ParserUtil::read(istream& is) {
		return is.get();
	}

	bool ParserUtil::hasNext(istream& is) {
		return next(is) != '\0';
	}

	void ParserUtil::skipWs(istream& is) {
		char ch;
		is.get(ch);

		while (isspace(ch)) {
			is.get(ch);
		}

		is.unget();
	}

	string ParserUtil::readWord(istream& is) {
		string result;

		while (isalpha(ParserUtil::next(is))) {
			result += read(is);
		}

		return result;
	}

	string ParserUtil::readNumber(istream& is) {
		string result;

		while (isdigit(ParserUtil::next(is))) {
			result += read(is);
		}

		return result;
	}
}
