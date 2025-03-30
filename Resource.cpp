#include "Resource.h"

#include <fstream>


using namespace std;


Resource::Resource()
{
	return;
}

int Resource::getId()
{
	return id;
}

char Resource::getType()
{
	return type;
}

ostream & operator<<(ostream & out, Resource * resource)
{
	

	return out;
}