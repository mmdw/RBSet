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

int main(int argc, char ** argv) {
	RBSet<int> set;

	set.put(2);
	set.put(4);
	set.put(6);
	set.put(1);
	set.put(3);
	set.put(8);

	set.checkInvariant();
	set.remove(2);
	set.checkInvariant();
	set.remove(3);
	set.checkInvariant();
	set.remove(1);
	set.checkInvariant();
	set.remove(4);
	set.checkInvariant();
	set.remove(6);
	set.checkInvariant();
	set.remove(8);
	set.checkInvariant();
	return 0;
}

