#pragma once

template <typename T>
class AbstractIterator {
public:
	virtual void next() = 0;
	virtual void prev() = 0;
	virtual bool hasNext() = 0;
	virtual T& item() = 0;

	virtual ~AbstractIterator() { }

protected:
	AbstractIterator() { }
};
