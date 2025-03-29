#pragma once

#include "Queue.h"

template <class T>
class EUWaitingQueue : public LinkedQueue<T>
{
private:

public:
	void insertSorted(T patient);
	int calcTreatmentLatency();
};