#include "Resource.h"

#include <fstream>


using namespace std;


Resource::Resource()
{
	return;
}

Resource::Resource(int Id, char Type, int xCap )
{
	id = Id;
	if (Type == 'E' || Type == 'U' || Type == 'X')
		type = Type;
	if (type == 'X')
		xCapacity = xCap;
}

int Resource::getId()
{
	return id;
}

char Resource::getType()
{
	return type;
}

int Resource::getxCapacity()
{
	return xCapacity;
}

ostream & operator<<(ostream & out, Resource * resource)
{
	

	return out;
}