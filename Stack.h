#pragma once


#include <iostream>
#include <string>


using namespace std;


# define STACK_SIZE 200
template<typename T>
class ArrayStack
{
protected:
	T items[STACK_SIZE];
	int top;
	int count;

public:

	ArrayStack()
	{
		top = -1;
		count = 0;
	}

	int getCount()
	{
		return count;
	}

	void print()
	{
		for (int i = 0; i < count; i++)
			cout << items[i]->getId();
	}

	int getCapacity()
	{
		return STACK_SIZE;
	}

	bool isEmpty() const
	{
		return top == -1;
	}

	bool push(const T & newEntry)
	{
		if (top == STACK_SIZE - 1) return false;	//Stack is FULL

		count++;

		top++;
		items[top] = newEntry;
		return true;
	}

	bool pop(T & TopEntry)
	{
		if (isEmpty()) return false;

		count--;

		TopEntry = items[top];
		top--;
		return true;
	}

	bool peek(T & TopEntry) const
	{
		if (isEmpty()) return false;

		TopEntry = items[top];
		return true;
	}

	//Copy constructor
	ArrayStack(const ArrayStack<T> & S)
	{
		for (int i = 0; i <= S.top; i++)
			items[i] = S.items[i];
		top = S.top;
	}
};