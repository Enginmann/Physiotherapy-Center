#pragma once

#include "Queue.h"

template <class T>
class EUWaitingQueue : public LinkedQueue<T>
{
private:
	int latency;
public:
	EUWaitingQueue()
	{
		latency = 0;
	}
	void insertSorted(T patient, int val)
	{
		if (!patient)
			return;

		this->count++;
		Node<T>* newNode = new Node<T>(patient);
		if (!this->frontPtr) //empty
		{
			this->frontPtr = newNode;
			this->backPtr = newNode;
			return;
		}

		if (val < this->frontPtr->getItem()->getPt()) //insert first
		{
			newNode->setNext(this->frontPtr);
			this->frontPtr = newNode;
			return;
		}
		// insert at exact position
		Node<T>* temp = this->frontPtr;
		while (temp->getNext() && temp->getNext()->getItem()->getPt() <= val)
		{
			temp = temp->getNext();
		}
		newNode->setNext(temp->getNext());
		temp->setNext(newNode);
		if (!newNode->getNext()) // update backptr if inserted in the end
			this->backPtr = newNode;
	}

	int calcTreatmentLatency()
	{
		if (this->count == 0)
			return 0;
		Node<T>* ptr = this->frontPtr;
		latency = 0;
		while (ptr)
		{
			latency += ptr->getItem()->getTt();
			ptr = ptr->getNext();
		}
		return latency;
	}
};