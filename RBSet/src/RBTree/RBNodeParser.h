/*
 * parserUtil.h
 *
 *  Created on: 03.03.2013
 *      Author: user
 */

#pragma once

#include <string>
#include <cstdlib>
#include <sstream>

#include "RBNode.h"

using std::istream;
using std::string;

namespace Tree {
	class RBNodeParser {
	public:
		RBNodeParser(istream& is);

		template<typename T>
		void parseRbNode(Node<T>** pp_node, size_t& count);

	private:
		istream& is;

		void error(const char* msg);
		char next(istream& is);
		char read(istream& is);
		bool hasNext(istream& is);
		string readWord(istream& is);
		string readNumber(istream& is);

		void skipWs(istream& is);

		template<typename T>
		void parseFieldSequence(Node<T>* p_node, size_t& count);
	};

	RBNodeParser::RBNodeParser(istream& is) : is(is) {

	}

	template<typename T>
	void RBNodeParser::parseRbNode(Node<T>** pp_node, size_t& count) {
		skipWs(is);
		if (next(is) == '{') {
			read(is); // {

			*pp_node = new Node<T>();
			++count;
			parseFieldSequence(*pp_node, count);

			skipWs(is);
			if (next(is) != '}') {
				error("expected: }");
			}

			read(is); // }
		} else {
			skipWs(is);
			string maybeNull = readWord(is);

			if (!maybeNull.compare("null")) {
				*pp_node = NULL;
			} else {
				error("expected: null or {");
			}
		}
	}

	template<typename T>
	void RBNodeParser::parseFieldSequence(Node<T>* p_node, size_t& count) {
		skipWs(is);
		if (!isalpha(next(is))) {
			error("expected: field name");
		}

		string fieldName = readWord(is);

		skipWs(is);
		if (next(is) != ':') {
			error("expected: :");
		}
		read(is); // :

		if (!fieldName.compare("color")) {
			skipWs(is);

			string color = readWord(is);
			if (!color.compare("black")) {
				p_node->color = BLACK;
			} else if (!color.compare("red")) {
				p_node->color = RED;
			} else {
				error("expected: red or black");
			}
		} else if (!fieldName.compare("key")) {
			skipWs(is);
			string keyString = readNumber(is);

			std::istringstream iss(keyString);
			T key;
			iss >> key;

			p_node->key = key;
		} else if (!fieldName.compare("left")) {
			skipWs(is);
			if (next(is) == '{' || next(is) == 'n') {
				parseRbNode(&p_node->left, count);

				if (p_node->left != NULL) {
					p_node->left->parent = p_node;
				}
			} else {
				error("expected: object");
			}
		} else if (!fieldName.compare("right")) {
			skipWs(is);
			if (next(is) == '{' || next(is) == 'n') {
				parseRbNode(&p_node->right, count);

				if (p_node->right != NULL) {
					p_node->right->parent = p_node;
				}
			} else {
				error("expected: object or null");
			}
		} else {
			error("expected: valid field name");
		}

		skipWs(is);
		if (next(is) == ',') {
			read(is); // ,

			parseFieldSequence(p_node, count);
		}

		string field = readWord(is);
	}

	void RBNodeParser::error(const char* msg) {
		std::cerr << msg;

		exit(-1);
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

	string RBNodeParser::readNumber(istream& is) {
		string result;

		while (isdigit(next(is))) {
			result += read(is);
		}

		return result;
	}
}
