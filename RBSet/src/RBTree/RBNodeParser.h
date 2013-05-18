#pragma once

#include <string>
#include <cstdlib>
#include <sstream>

#include "ItemArray.h"
#include "RBNode.h"

class RBSetParserException;

using std::istream;
using std::string;

namespace Tree {
	class RBNodeParser {
	public:
		RBNodeParser(istream& is);

		template<typename T>
		typename ItemArray<Node<T> >::ItemId parseRbNode(size_t& count, ItemArray<Node<T> >& ia);

	private:
		istream& is;

		char next(istream& is);
		char read(istream& is);
		bool hasNext(istream& is);
		string readWord(istream& is);
		string readKey(istream& is);

		void skipWs(istream& is);

		template<typename T>
		void parseFieldSequence(typename ItemArray<Node<T> >::ItemId p_node, size_t& count, ItemArray<Node<T> >& ia);
	};

	RBNodeParser::RBNodeParser(istream& is) : is(is) {

	}

	template<typename T>
	typename ItemArray<Node<T> >::ItemId RBNodeParser::parseRbNode(size_t& count, ItemArray<Node<T> >& ia) {
		skipWs(is);
		typename ItemArray<Node<T> >::ItemId p_node = ItemArray<Node<T> >::Null;

		if (next(is) == '{') {
			read(is); // {

			p_node = ia.place(Node<T>());
			++count;
			parseFieldSequence(p_node, count, ia);

			skipWs(is);
			if (next(is) != '}') {
				throw RBSetParserException("expected: }");
			}

			read(is); // }
		} else {
			skipWs(is);
			string maybeNull = readWord(is);

			if (!maybeNull.compare("null")) {
				p_node = ItemArray<Node<T> >::Null;
			} else {
				throw RBSetParserException("expected: null or {");
			}
		}

		return p_node;
	}

	template<typename T>
	void RBNodeParser::parseFieldSequence(typename ItemArray<Node<T> >::ItemId p_node, size_t& count, ItemArray<Node<T> >& ia) {
		skipWs(is);
		if (!isalpha(next(is))) {
			throw RBSetParserException("expected: field name");
		}

		string fieldName = readWord(is);

		skipWs(is);
		if (next(is) != ':') {
			throw RBSetParserException("expected: :");
		}
		read(is); // :

		if (!fieldName.compare("color")) {
			skipWs(is);

			string color = readWord(is);
			if (!color.compare("black")) {
				ia[p_node].color = BLACK;
			} else if (!color.compare("red")) {
				ia[p_node].color = RED;
			} else {
				throw RBSetParserException("expected: red or black");
			}
		} else if (!fieldName.compare("key")) {
			skipWs(is);
			string keyString = readKey(is);

			std::istringstream iss(keyString);
			T key;
			iss >> key;

			ia[p_node].key = key;
		} else if (!fieldName.compare("left")) {
			skipWs(is);
			if (next(is) == '{' || next(is) == 'n') {
				typename ItemArray<Node<T> >::ItemId leftId = parseRbNode(count, ia);

				if (leftId != ItemArray<Node<T> >::Null) {
					ia[leftId].parent = p_node;
				}

				ia[p_node].left = leftId;
			} else {
				throw RBSetParserException("expected: object");
			}
		} else if (!fieldName.compare("right")) {
			skipWs(is);
			if (next(is) == '{' || next(is) == 'n') {
				typename ItemArray<Node<T> >::ItemId rightId = parseRbNode(count, ia);

				if (rightId != ItemArray<Node<T> >::Null) {
					ia[rightId].parent = p_node;
				}
				ia[p_node].right = rightId;
			} else {
				throw RBSetParserException("expected: object or null");
			}
		} else {
			throw RBSetParserException("expected: valid field name");
		}

		skipWs(is);
		if (next(is) == ',') {
			read(is); // ,

			parseFieldSequence(p_node, count, ia);
		}

		string field = readWord(is);
	}

	char RBNodeParser::next(istream& is) {
		char ch;

		is.get(ch);
		is.unget();

		return ch;
	}

	char RBNodeParser::read(istream& is) {
		return is.get();
	}

	bool RBNodeParser::hasNext(istream& is) {
		return next(is) != '\0';
	}

	void RBNodeParser::skipWs(istream& is) {
		char ch;
		is.get(ch);

		while (isspace(ch)) {
			is.get(ch);
		}

		is.unget();
	}

	string RBNodeParser::readWord(istream& is) {
		string result;

		while (isalpha(next(is))) {
			result += read(is);
		}

		return result;
	}

	string RBNodeParser::readKey(istream& is) {
		string result;

		while (next(is) != ',' && next(is) != '}') {
			result += read(is);
		}

		return result;
	}
}
