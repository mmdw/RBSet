/*
 * parserUtil.h
 *
 *  Created on: 03.03.2013
 *      Author: user
 */

#pragma once

#include <istream>
#include <string>

using std::istream;
using std::string;

namespace Tree {
	class ParserUtil {
	public:
		static void error(const char* msg);
		static char next(istream& is);
		static char read(istream& is);
		static bool hasNext(istream& is);
		static string readWord(istream& is);
		static string readNumber(istream& is);

		static void skipWs(istream& is);
	};
}
