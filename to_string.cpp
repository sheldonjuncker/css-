/*
	A little library for converting numbers to strings.
*/

#include <string>
#include <sstream>

std::string String(int num)
{
	std::stringstream s;
	s << num;
	return s.str();
}

std::string String(double num)
{
	std::stringstream s;
	s << num;
	return s.str();
}