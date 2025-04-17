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
	bool cancel2(T & patient)
	{
		if (this->count == 0)
			return false;

		Node<T> * temp = this->frontPtr;
		bool isPossible = false;
		while (temp)
		{
			if (temp->getItem()->getTreatment())
			temp = temp->getNext();
		}


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
};