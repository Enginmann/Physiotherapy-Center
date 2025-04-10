#pragma once

#include "PriQueue.h"

template <class T>
class EarlyPriQueue : public priQueue<T>
{
private:

public:
	bool reschedule() {
		//check the same null in xwaiting 
		int index = rand() % count;
		Node<T>* ptr = frontPtr;
		for (int i = 0; i < index - 1; i++) {
			ptr = ptr->getNext();
		}
		Node<T>* ptr2 = ptr->getNext();
		ptr->setNext(ptr2->getNext());
		ptr2->setNext(nullptr);


		///////
	    index = rand() % 999999999;
		enqueue(ptr2,-index);
		
	}
};

