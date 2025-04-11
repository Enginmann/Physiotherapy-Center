#pragma once


class Resource
{
protected:
	int id;
	char type;

public:
	Resource(int Id, char Type);

	int getId();

	char getType();

	virtual void print(int count);
};