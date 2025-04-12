#pragma once

#include "PriQueue.h"

template <class T>
class EarlyPriQueue : public priQueue<T>
{
private:

public:
    bool reschedule(int& maxPt) {
        if (this->count == 0 || !this->head)
            return false;

        // Select random node
        int index = rand() % this->count;
        priNode<T>* prev = nullptr;
        priNode<T>* curr = this->head;

        // traverse to the selected node
        for (int i = 0; i < index; i++) {
            prev = curr;
            curr = curr->getNext();
        }

        // remove the node from the list
        if (prev)
            prev->setNext(curr->getNext());
        else
            this->head = curr->getNext();

        // decrement count
        this->count--;

        // Get the patient and current Pt
        int temp;
        T patient = curr->getItem(temp);
        int oldPt = patient->getPt();

        // edge cases
        if (oldPt == maxPt)
            maxPt += 10;
        else if (maxPt < oldPt)
            maxPt = oldPt + 10;

        // must newPt > oldPt
        int newPt = oldPt + 1 + rand() % (maxPt - oldPt);
        patient->setPt(newPt);

        // reinsert with newPt
        this->enqueue(patient, -newPt);  

        // delete curr
        curr->setNext(nullptr);
        delete curr;
        return true;
    }
};

