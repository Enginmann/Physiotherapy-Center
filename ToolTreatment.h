#pragma once


struct Tooltreatment
{
	char type;
	int duration;

	Tooltreatment(char type, int duration)
	{
		this->duration = duration;
		this->type = type;
	}
};