#include <string>
#include <iostream>
#include "tcss.h"

void TCSS::warning(std::string s)
{
	if(!issueWarnings)
		return;
		
	std::cout << "Warning: " << s << ".\n";
}

bool TCSS::issueWarnings = 1;
