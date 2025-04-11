#pragma once

#include "Queue.h"

template <class T>
class EUWaitingQueue : public LinkedQueue<T>
{
private:
	int latency;
public:
	EUWaitingQueue() {
		latency = 0;
	}
	void insertSorted(T patient, int val)
	{
		Node<T>* newNode = new Node<T>(patient);
		if (!this->frontPtr) //empty
		{
			this->frontPtr = this->backPtr = newNode;
			return;
		}
		if (val <= this->frontPtr->getItem()->getPt()) //insert first
		{
			newNode->setNext(this->frontPtr);
			this->frontPtr = newNode;
			return;
		}
		// insert at exact position
		Node<T>* temp = this->frontPtr;
		while (temp->getNext() && temp->getNext()->getItem() < val)
		{
			temp = temp->getNext();
		}
		newNode->setNext(temp->getNext());
		temp->setNext(newNode);
		if (!newNode->getNext()) // update backptr if inserted at the last
			this->backPtr = newNode;
		
	}

	int calcTreatmentLatency()
	{
		return 0;
	}
};