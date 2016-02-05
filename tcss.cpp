struct TCSS
{
	static bool issueWarnings;
	
	static void warning(std::string s)
	{
		if(!issueWarnings)
			return;
		
		std::cout << "Warning: " << s << ".\n";
	}
};

bool TCSS::issueWarnings = 1;
