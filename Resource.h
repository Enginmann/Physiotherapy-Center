#pragma once


class Resource
{
private:
	int id;
	char type;

public:
	Resource(int Id, char Type);

	int getId();

	char getType();

	void print();
};