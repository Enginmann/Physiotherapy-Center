#pragma once


class Resource
{
protected:
	int id;
	char type;
	int pFreeFailure;
	int pBusyFailure;
	int maintenence;

public:
	Resource(int Id, char Type, int free,int busy,int maint);

	int getId();

	char getType();

	int getFree();

	int getBusy();

	int getmaint();

	virtual void print(int count = 0);
};