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