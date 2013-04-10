#pragma once
#include "AbstractIterator.h"

template <typename T>
class AbstractSet {
public:
	virtual void 		put						(const T&value) 		= 0;
	virtual size_t 		size					() const 				= 0;
	virtual void		remove					(const T& value) 		= 0;

	virtual
	AbstractIterator<T>* iterator				() 						= 0;

	virtual	bool 		contains				(const T& value) const 	= 0;

	virtual 			~AbstractSet			() { }
private:
};
