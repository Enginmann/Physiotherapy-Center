#pragma once

#include "EUWaitingQueue.h"
#include <random>
#include <ctime>

template <class T>
class XWaitingQueue : public EUWaitingQueue<T>
{
private:

public:
	bool cancel(T & patient)
	{
		//check is needed
		int index = rand() % this->count;
		Node<T>* ptr = this->frontPtr;
		for (int i = 0; i < index - 1; i++) {
			ptr = ptr->getNext();
		}
		Node<T>* ptr2 = ptr->getNext();
		ptr->setNext(ptr2->getNext());
		ptr2->setNext(nullptr);
		patient = ptr2->getItem();
		return 1;
	} 
};

