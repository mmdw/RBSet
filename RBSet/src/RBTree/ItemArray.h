/*
 * ItemArray.h
 *
 *  Created on: 15.05.2013
 *      Author: user
 */

#pragma once

#include <vector>
#include <map>
#include <cassert>

#include <iostream>

template<typename T>
class ItemArray {
public:
	static const unsigned Null;
	typedef unsigned ItemId;

	ItemArray();
	virtual ~ItemArray();

	unsigned place(const T& item);
	void free(unsigned id);

	T& operator[](unsigned id);
	const T& operator[](unsigned id) const;

	void debug(std::ostream& os);

private:
	std::vector<T> items;
	std::map<unsigned, unsigned> freeBlocks;
};

template<typename T>
ItemArray<T>::ItemArray() : items(1) {
	items[Null] = T();
}

template<typename T>
const unsigned ItemArray<T>::Null(0);

template<typename T>
ItemArray<T>::~ItemArray() {
}

template<typename T>
unsigned ItemArray<T>::place(const T& item) {
	std::map<unsigned, unsigned>::iterator it = freeBlocks.begin();
	if (it == freeBlocks.end()) {
		unsigned idx = items.size();

		items.resize(items.size() + 1);
		items[idx] = item;

		return idx;
	}

	unsigned idx = it->first + it->second - 1;

	if (it->second == 1) {
		freeBlocks.erase(it);
	} else {
		--(it->second);
	}

	items[idx] = item;

	return idx;
}

template<typename T>
void ItemArray<T>::free(unsigned id) {
	std::map<unsigned, unsigned>::iterator it = freeBlocks.begin(), prev = freeBlocks.end();

	for (/* */; it != freeBlocks.end(); ++it) {
		bool beg = (id + 1 == it->first);
		bool end = (id == it->first + it->second);

		if (beg) {
			size_t s = it->second + 1;

			freeBlocks.erase(it);
			freeBlocks.insert(std::pair<int, int>(id, s));

			return;
		} else
		if (end) {
			std::map<unsigned, unsigned>::iterator next = freeBlocks.find(id + it->second);
			if (next != freeBlocks.end()) {
				it->second += next->second;
				freeBlocks.erase(next);
			}

			++it->second;
			return;
		}

		prev = it;
	}

	freeBlocks.insert(std::pair<unsigned, unsigned>(id, 1));
}

template<typename T>
void ItemArray<T>::debug(std::ostream& os)  {
	os << "data: [";
	for (unsigned i = 0; i != items.size(); ++i) {
		os << items.at(i);

		if (i != items.size() - 1) {
			os << ", ";
		}
	}
	os << ']' << std::endl;

	os << "free: [";
	for (std::map<unsigned, unsigned>::iterator it = freeBlocks.begin(); it != freeBlocks.end(); ++it) {
		if (it != freeBlocks.begin()) {
			std::cout << ", ";
		}

		os << it->first << ":" << it->first + it->second;
	}
	os << ']' << std::endl;
}

template<typename T>
T& ItemArray<T>::operator[](unsigned id) {
	assert(id != Null);
	return items[id];
}

template<typename T>
const T& ItemArray<T>::operator[](unsigned id) const {
	assert(id != Null);
	return items[id];
}
