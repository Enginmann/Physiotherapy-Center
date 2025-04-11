#pragma once

#include "PriQueue.h"

template <class T>
class EarlyPriQueue : public priQueue<T>
{
private:

public:
	bool reschedule(int maxPt)
	{
		if (this->count == 0)
			return false;

		int index = rand() % this->count;

		if (!this->head)
			return false;
		
		priNode<T>* ptr = this->head;
		priNode<T>* ptr2 = ptr->getNext();

		if (this->head->getNext())
		{
			for (int i = 0; i < index - 1; i++)
				ptr = ptr->getNext();

			ptr->setNext(ptr2->getNext());
			ptr2->setNext(nullptr);
			int temp = 0;
			int oldPt = ptr2->getItem(temp)->getPt();
			int newPt = -1;
			if (oldPt + 1 == maxPt)
				maxPt += 10;
			while (oldPt >= newPt)
				newPt = rand() % maxPt;
			ptr2->getItem(temp)->setPt(newPt);
		
			this->enqueue(ptr2->getItem(temp), -newPt);
		}
		else /// head only
		{
			int temp = 0;
			int oldPt = this->head->getItem(temp)->getPt();
			int newPt = -1;
			if (oldPt + 1 == maxPt)
				maxPt += 10;
			while (oldPt >= newPt)
				newPt = rand() % maxPt;
			this->head->getItem(temp)->setPt(newPt);
		}

		return true;
	}
};

