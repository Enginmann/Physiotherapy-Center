#pragma once


class Resource
{
private:
	int id;
	int type; // 0->U, 1-> E, 2->X

public:
	int getId();
	int getType();
};