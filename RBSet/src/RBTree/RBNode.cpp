/*
 * RBNode.cpp
 *
 *  Created on: 03.03.2013
 *      Author: user
 */
#include "RBNode.h"
namespace Tree {
	ostream& operator<<(ostream& os, const NodeColor& color) {
		switch (color) {
		case BLACK: os << "'black'";
			break;
		case RED: os << "'red'";
			break;
		}
		return os;
	}

	void newline(ostream& os, int tabs) {
		os << std::endl;

		for (int i = 0; i < 2 * tabs; ++i) {
			os << " ";
		}
	}
}
