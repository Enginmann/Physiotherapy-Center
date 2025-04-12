#pragma once

#include "PriQueue.h"

template <class T>
class EarlyPriQueue : public priQueue<T>
{
private:

public:
	//bool reschedule(int & maxPt)
	//{
	//	if (this->count == 0)
	//		return false;

	//	int index = rand() % this->count;

	//	if (!this->head)//has no use
	//		return false;
	//	
	//	priNode<T>* ptr = this->head;
	//	priNode<T>* ptr2 = nullptr;

	//	if (index == 0)
	//	{
	//		this->head = this->head->getNext();
	//		ptr->setNext(nullptr);
	//		int temp = 0;
	//		int oldPt = ptr->getItem(temp)->getPt();
	//		int newPt = -1;
	//		if (oldPt == maxPt)
	//			maxPt += 10;
	//		while (oldPt >= newPt)
	//			newPt = rand() % maxPt;
	//		ptr->getItem(temp)->setPt(newPt);
	//		//this->count--;
	//		this->enqueue(ptr->getItem(temp), -newPt);
	//	}
	//	else if (this->head->getNext())
	//	{
	//		for (int i = 0; i < index - 1; i++)
	//			ptr = ptr->getNext();

	//		ptr2 = ptr->getNext();
	//		ptr->setNext(ptr2->getNext());
	//		ptr2->setNext(nullptr);
	//		int temp = 0;
	//		int oldPt = ptr2->getItem(temp)->getPt();
	//		int newPt = -1;
	//		if (oldPt == maxPt)
	//			maxPt += 10;
	//		while (oldPt >= newPt)
	//			newPt = rand() % maxPt;
	//		ptr2->getItem(temp)->setPt(newPt);
	//		//this->count--;

	//		this->enqueue(ptr2->getItem(temp), -newPt);
	//	}
	//	else /// head only
	//	{
	//		int temp = 0;
	//		int oldPt = this->head->getItem(temp)->getPt();
	//		int newPt = -1;
	//		if (oldPt == maxPt)
	//			maxPt += 10;
	//		while (oldPt >= newPt)
	//			newPt = rand() % maxPt;
	//		this->head->getItem(temp)->setPt(newPt);
	//	}

	//	return true;
	//}


	bool reschedule(int& maxPt) {
		Patient* patient = nullptr;
		if (!this->count)
			return 0;
		int temp = 0;
		int index = rand() % this->count;
		if (!index) {
			this->dequeue(patient, temp);
			int npt = patient->getPt() + rand() % (maxPt - patient->getPt()) + 1;
			patient->setPt(npt);
			this->enqueue(patient, -npt);
			return 1;

		}
		priNode<T>* ptr = this->head;
		for (int i = 0; i < index - 1; i++) {
			ptr = ptr->getNext();
		}

		priNode<T>* ptr2 = ptr->getNext();
		ptr->setNext(ptr2->getNext());
		if (ptr2->getNext())
			ptr2->setNext(nullptr);
		patient = ptr2->getItem(temp);
		int npt = patient->getPt() + rand() % (maxPt - patient->getPt()) + 1;
		patient->setPt(npt);
		this->count--;
		this->enqueue(patient, -npt);

		return 1;

	}
};

