#pragma once

#include "PriQueue.h"

template <class T>
class EarlyPriQueue : public priQueue<T>
{
private:

public:
    bool reschedule(int& maxPt)
    {
        if (this->count == 0)
            return false;
        int index = rand() % this->count;
        priNode<T>* prev = nullptr;
        priNode<T>* ptr = this->head;
        for (int i = 0; i < index; i++)
        {
            prev = ptr;
            ptr = ptr->getNext();
        }
        if (prev)
            prev->setNext(ptr->getNext());
        else
            this->head = ptr->getNext();
        this->count--;
        int temp = 0;
        int oldPt = ptr->getItem(temp)->getPt();
        if (oldPt == maxPt)
            maxPt += 10;
        if (oldPt > maxPt)
            maxPt = oldPt + 10;
        int newPt = oldPt + 1 + rand() % maxPt;
        ptr->getItem(temp)->setPt(newPt);

        this->enqueue(ptr->getItem(temp), -newPt);
        return true;
    }
};

