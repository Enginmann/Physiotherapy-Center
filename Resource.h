#pragma once


class Resource
{
private:
	int type; // 0->U, 1-> E, 2->X
	int id;

public:
	int getType();
	int getId();
};