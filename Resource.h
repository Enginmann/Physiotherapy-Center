#pragma once


class Resource
{
private:
	int id;
	char type;
	int xCapacity;
public:
	Resource();
	Resource(int Id,char Type, int xCap = 0);
	int getId();
	char getType();
	int getxCapacity();
};