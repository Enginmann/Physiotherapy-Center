#include "Resource.h"

#include <fstream>


using namespace std;


Resource::Resource(int Id, char Type)
{
	id = Id;
	type = Type;
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
	/// Later

	return out;
}