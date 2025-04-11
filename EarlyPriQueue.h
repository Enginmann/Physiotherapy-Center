#pragma once

#include "PriQueue.h"

template <class T>
class EarlyPriQueue : public priQueue<T>
{
private:

public:
	bool reschedule()
	{
		//check the same null in xwaiting 
		int index = rand() % this->count;
		priNode<T>* ptr = this->head;
		for (int i = 0; i < index - 1; i++)
		{
			ptr = ptr->getNext();
		}
		priNode<T>* ptr2 = ptr->getNext();
		ptr->setNext(ptr2->getNext());
		ptr2->setNext(nullptr);


		///////
	    int newPt = rand() % 999999999;
		int temp = 0;
		priQueue<T>::enqueue(ptr2->getItem(temp), -newPt);
		return true;
	}
};

