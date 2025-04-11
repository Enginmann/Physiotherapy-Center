#include "Resource.h"

#include <iostream>


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

 void  Resource::print(int count) 
{
	cout << id;

}

//ostream & operator<<(ostream & out, Resource * resource)
//{
//	/// Later
//
//	return out;
//}