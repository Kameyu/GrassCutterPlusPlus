#include "Position.h"

#include <iostream>


Position::Position(float x, float y)
{
	set(x, y);
}

Position::Position(float x, float y, float z)
{
	set(x, y, z);
}

Position* Position::set(float x, float y)
{
	this->x = x;
	this->y = y;
	return this;
}

Position* Position::set(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
	return this;
}

Position* Position::set(const std::vector<float>& pos)
{
	switch (pos.size())
	{
	default:
		return nullptr;
	case 3:
		this->z = pos[2];
	case 2:
		this->y = pos[1];
	case 1:
		this->x = pos[0];
	}
	return this;
}

Position* Position::set(std::string& pos, const std::string& delim = ",")
{
	std::vector<float> v;
	size_t p = 0;
	while ((p = pos.find(',')) != std::string::npos)
	{
		 v.push_back(std::stof(pos.substr(0, p)));
		 pos.erase(0, p+delim.length());
	}
	return set(v);
	
}
