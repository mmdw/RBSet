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

template<typename T>
class ItemArray {
public:
	ItemArray();
	virtual ~ItemArray();

	unsigned alloc(const T& item);
	void free(unsigned id);
	void debug();
	T& operator[](unsigned id);

	static const unsigned null;
	typedef unsigned ItemId;

private:
	std::vector<T> items;
	std::map<unsigned, unsigned> freeBlocks;
};

template<typename T>
ItemArray<T>::ItemArray() {
	items.resize(items.size() + 1);
	items[null] = T();
}

template<typename T>
const unsigned ItemArray<T>::null(0);

template<typename T>
ItemArray<T>::~ItemArray() {
}


template<typename T>
unsigned ItemArray<T>::alloc(const T& item) {
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
void ItemArray<T>::debug()  {
	std::cout << "data: [";
	for (unsigned i = 0; i != items.size(); ++i) {
		std::cout << items.at(i);

		if (i != items.size() - 1) {
			std::cout << ", ";
		}
	}
	std::cout << ']' << std::endl;

	std::cout << "free: [";
	for (std::map<unsigned, unsigned>::iterator it = freeBlocks.begin(); it != freeBlocks.end(); ++it) {
		if (it != freeBlocks.begin()) {
			std::cout << ", ";
		}

		std::cout << it->first << ":" << it->first + it->second;
	}
	std::cout << ']' << std::endl;
}

template<typename T>
T& ItemArray<T>::operator [](unsigned id) {
	assert(id != null);
	return items.at(id);
}
