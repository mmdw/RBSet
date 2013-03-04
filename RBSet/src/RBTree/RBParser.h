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

template<typename T>
void parseFieldSequence(std::istream& is, Node<T>* p_node) {
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

		istringstream iss(keyString);
		T key;
		iss >> key;

		p_node->key = key;
	} else if (!fieldName.compare("left")) {
		skipWs(is);
		if (next(is) == '{' || next(is) == 'n') {
			parseRbNode(is, &p_node->left);

			if (p_node->left != NULL) {
				p_node->left->parent = p_node;
			}
		} else {
			error("expected: object");
		}
	} else if (!fieldName.compare("right")) {
		skipWs(is);
		if (next(is) == '{' || next(is) == 'n') {
			parseRbNode(is, &p_node->right);

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

		parseFieldSequence(is, p_node);
	}

	string field = readWord(is);
}

template<typename T>
void parseRbNode(std::istream& is, Node<T>** pp_node) {
	skipWs(is);
	if (next(is) == '{') {
		read(is); // {

		*pp_node = new Node<T>();
		parseFieldSequence(is, *pp_node);

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
#endif /* RBPARSER_H_ */
