/*
	Some general functions used by the TCSS preprocessor
*/
struct TCSS
{
	/*
		If true, the warning function will issue warnings
		If false, warnings will be suppressed
	*/
	static bool issueWarnings;
	
	static void warning(std::string s);
};
