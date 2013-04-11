/*
 * RbParser.h
 *
 *  Created on: 03.03.2013
 *      Author: user
 */

#ifndef RBPARSER_H_
#define RBPARSER_H_

#include <ostream>
#include <sstream>

#include "parserUtil.h"

using std::string;
using std::istringstream;

namespace Tree {
	template<typename T>
	void parseFieldSequence(std::istream& is, Node<T>* p_node, size_t& count);

	template<typename T>
	void parseRbNode(std::istream& is, Node<T>** pp_node, size_t& count) {
		ParserUtil::skipWs(is);
		if (ParserUtil::next(is) == '{') {
			ParserUtil::read(is); // {

			*pp_node = new Node<T>();
			++count;
			parseFieldSequence(is, *pp_node, count);

			ParserUtil::skipWs(is);
			if (ParserUtil::next(is) != '}') {
				ParserUtil::error("expected: }");
			}

			ParserUtil::read(is); // }
		} else {
			ParserUtil::skipWs(is);
			string maybeNull = ParserUtil::readWord(is);

			if (!maybeNull.compare("null")) {
				*pp_node = NULL;
			} else {
				ParserUtil::error("expected: null or {");
			}
		}
	}
	template<typename T>
	void parseFieldSequence(std::istream& is, Node<T>* p_node, size_t& count) {
		ParserUtil::skipWs(is);
		if (!isalpha(ParserUtil::next(is))) {
			ParserUtil::error("expected: field name");
		}

		string fieldName = ParserUtil::readWord(is);

		ParserUtil::skipWs(is);
		if (ParserUtil::next(is) != ':') {
			ParserUtil::error("expected: :");
		}
		ParserUtil::read(is); // :

		if (!fieldName.compare("color")) {
			ParserUtil::skipWs(is);

			string color = ParserUtil::readWord(is);
			if (!color.compare("black")) {
				p_node->color = BLACK;
			} else if (!color.compare("red")) {
				p_node->color = RED;
			} else {
				ParserUtil::error("expected: red or black");
			}
		} else if (!fieldName.compare("key")) {
			ParserUtil::skipWs(is);
			string keyString = ParserUtil::readNumber(is);

			istringstream iss(keyString);
			T key;
			iss >> key;

			p_node->key = key;
		} else if (!fieldName.compare("left")) {
			ParserUtil::skipWs(is);
			if (ParserUtil::next(is) == '{' || ParserUtil::next(is) == 'n') {
				parseRbNode(is, &p_node->left, count);

				if (p_node->left != NULL) {
					p_node->left->parent = p_node;
				}
			} else {
				ParserUtil::error("expected: object");
			}
		} else if (!fieldName.compare("right")) {
			ParserUtil::skipWs(is);
			if (ParserUtil::next(is) == '{' || ParserUtil::next(is) == 'n') {
				parseRbNode(is, &p_node->right, count);

				if (p_node->right != NULL) {
					p_node->right->parent = p_node;
				}
			} else {
				ParserUtil::error("expected: object or null");
			}
		} else {
			ParserUtil::error("expected: valid field name");
		}

		ParserUtil::skipWs(is);
		if (ParserUtil::next(is) == ',') {
			ParserUtil::read(is); // ,

			parseFieldSequence(is, p_node, count);
		}

		string field = ParserUtil::readWord(is);
	}
}
#endif /* RBPARSER_H_ */
