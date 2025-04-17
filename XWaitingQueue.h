#pragma once

#include "EUWaitingQueue.h"
#include <random>
#include <ctime>

template <class T>
class XWaitingQueue : public EUWaitingQueue<T>
{
private:

public:
	/// Phase 1.2 Cancel
	bool cancel(T & patient)
	{
		if (this->count == 0)
			return false;

		if (this->count == 1)
		{
			patient = this->frontPtr->getItem();
			delete this->frontPtr;

			this->frontPtr = nullptr;
			this->backPtr = nullptr;
			
			this->count--;
			return true;
		}

		Node<T> * ptr = this->frontPtr;
		Node<T> * ptr2 = this->frontPtr;
		int index = rand() % this->count;

		for (int i = 0; i < index - 1; i++)
			ptr = ptr->getNext();

		ptr2 = ptr->getNext();

		if (index == this->count - 1)
		{
			ptr->setNext(nullptr);
			this->backPtr = ptr;
			patient = ptr2->getItem();
			this->count--;
			return true;
		}
		return true;
	} 

	/// Phase 2 Cancel
	bool cancel_ph2(T& patient)
	{
		if (this->count == 0)
			return false;

		if (this->count == 1)
		{
			if (!this->frontPtr->getItem()->canCancel())
				return 0;
			patient = this->frontPtr->getItem();

			delete this->frontPtr;

			this->frontPtr = nullptr;
			this->backPtr = nullptr;

			this->count--;
			return true;
		}

		Node<T>* ptr = this->frontPtr;
		Node<T>* ptr2 = this->frontPtr;

		Node<T>* tempPtr = this->frontPtr;
		int * tempContainer = new int[this->count];
		int tempIndex = 0; /// to track the index of the patients that can cancel inside the queue
		int tempCount = 0; /// to track the index of the indecies stored in the temp container

		while (tempPtr)
		{
			if (tempPtr->getItem()->canCancel())		// Example
			{											// [p0, p1, p2, p3], where p0 and p2 only can cancel
				tempContainer[tempCount] = tempIndex;	// therefore tempCount = 2, tempContainer = [0, 2] (the indecies of p0 and p2 respectively)
				tempCount++;
			}
			tempPtr = tempPtr->getNext();
			tempIndex++;
		}

		int index = tempContainer[rand() % tempCount];

		for (int i = 0; i < index - 1; i++)
			ptr = ptr->getNext();

		ptr2 = ptr->getNext();
		if (!ptr2->getItem()->canCancel())
			return 0;

		if (index == this->count - 1)
		{
			ptr->setNext(nullptr);
			this->backPtr = ptr;
			patient = ptr2->getItem();
			this->count--;
			return true;
		}
		return true;
	}
};