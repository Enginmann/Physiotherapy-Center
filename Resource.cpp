#include "Resource.h"

#include <iostream>


using namespace std;


Resource::Resource(int Id, char Type, int free, int busy, int maint)
{
	id = Id;
	type = Type;
	pFreeFailure = free;
	pBusyFailure = busy;
	maintenence = maint;
	isFreeFailed = false;
	isBusyFailed = false;
}

int Resource::getId()
{
	return id;
}

char Resource::getType()
{
	return type;
}

int Resource::getFree()
{
	return pFreeFailure;
}

int Resource::getBusy()
{
	return pBusyFailure;
}

int Resource::getmaint()
{
	return maintenence;
}

void Resource::setBusy()
{
	isBusyFailed = true;
}

void Resource::setFree()
{
	isFreeFailed = true;
}

bool Resource::getIsFree()
{
	return isFreeFailed;
}

bool Resource::getIsBusy()
{
	return isBusyFailed;
}

void Resource::print(int count) 
{
	const char * resetColor = "\033[0m";
	const char * blueColor = "\033[34m";
	const char * cyanColor = "\033[36m";

	cout << resetColor;
	if (type == 'E')
		cout << blueColor;
	else if (type == 'U')
		cout << cyanColor;
	
	cout << type << id;

	cout << resetColor;
}