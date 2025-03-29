#pragma once

#include "EUWaitingQueue.h"

template <class T>
class XWaitingQueue : public EUWaitingQueue<T>
{
private:

public:
	void cancel(); /// change return type and parameters later
};
