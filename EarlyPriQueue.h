#pragma once

#include "PriQueue.h"

template <class T>
class EarlyPriQueue : public priQueue<T>
{
private:

public:
	void reschedule();
};

template<class T>
inline void EarlyPriQueue<T>::reschedule()
{

}
