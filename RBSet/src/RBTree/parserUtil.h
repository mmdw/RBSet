/*
 * parserUtil.h
 *
 *  Created on: 03.03.2013
 *      Author: user
 */

#ifndef PARSERUTIL_H_
#define PARSERUTIL_H_

#include <istream>
#include <string>

using std::istream;
using std::string;

void error(const char* msg);
char next(istream& is);
char read(istream& is);
bool hasNext(istream& is);
string readWord(istream& is);
string readNumber(istream& is);

void skipWs(istream& is);

#endif /* PARSERUTIL_H_ */
