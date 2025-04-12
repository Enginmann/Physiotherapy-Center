#pragma once

#include "PriQueue.h"

template <class T>
class EarlyPriQueue : public priQueue<T>
{
private:

public:
    bool reschedule(int& maxPt)
    {
        return true;
    }
};

