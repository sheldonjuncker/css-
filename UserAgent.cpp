#include <sstream>
#include <map>
#include <string>

//Agent Info Identifers
std::map <std::string, int> agent_idents;

struct AgentInfo
{
	std::string platform;
	std::string browser;
	std::string version;
	std::string platform_type;
};

struct UserAgent
{
	static AgentInfo *getAgentInfo()
	{
		AgentInfo *agent = new AgentInfo;
		
		char *cua = getenv("HTTP_USER_AGENT");
		std::string ua = "";
		if(cua != NULL)
			ua = cua;
	
		std::string cmd = PHP_BINARY + " " + PATH_TO_TCSS + "/user_agent.php \"" + ua + "\"";
	
		FILE *fp = popen(cmd.c_str(), "r");
		
		char buf[1024];
		char *result = fgets(buf, 1024, fp);
		pclose(fp);

		std::string platform = "UnknownPlatform";
		std::string browser = "UnknownBrowser";
		std::string platform_type = "UnknownType";
		std::string version = "0";
		
		//Succesfull: read results from PHP script
		if(result != NULL)
		{
			std::string result_string = result;
			std::stringstream data(result_string);
		
			std::getline(data,agent->platform,':');
			std::getline(data,agent->browser,':');
			std::getline(data,agent->version,':');
			
			//Platform
			if(!strncmp(agent->platform.c_str(), "Windows", 7))
			{
				platform = "Windows";
				if(agent->platform == "Windows")
					platform_type = "Desktop";
				else
					platform_type = "Mobile";
			}
			else if(agent->platform == "Linux")
			{
				platform = "Linux";
				platform_type = "Desktop";
			}
			else if(agent->platform == "Android")
			{
				platform = "Android";
				platform_type = "Mobile";
			}
			else if(agent->platform == "Kindle")
			{
				platform = "Kindle";
				platform_type = "Mobile";
			}
			else if(agent->platform == "Macintosh")
			{
				platform = "Apple";
				platform_type = "Desktop";
			}
			else if(agent->platform == "iPod")
			{
				platform = "Apple";
				platform_type = "Mobile";
			}
			else if(agent->platform == "iPhone")
			{
				platform = "Apple";
				platform_type = "Mobile";
			}
			else if(agent->platform == "iPad")
			{
				platform = "Apple";
				platform_type = "Mobile";
			}
			else if(!strncmp(agent->platform.c_str(), "Xbox", 4))
			{
				platform = "Xbox";
				platform_type = "Console";
			}
			else if(!strncmp(agent->platform.c_str(), "Nintendo", 8))
			{
				platform = "Nintendo";
				platform_type = "Console";
			}
			else if(!strncmp(agent->platform.c_str(), "PlayStation", 11))
			{
				platform = "PlayStation";
				platform_type = "Console";
			}
			else
			{
				platform = "UnknownOS";
				platform_type = "UnknownType";
			}
			
			//Browser
			if(agent->browser == "Firefox")
				browser = "Firefox";
			else if(agent->browser == "Chrome")
				browser = "Chrome";
			else if(agent->browser == "MSIE")
				browser = "IE";
			else if(agent->browser == "Safari")
				browser = "Safari";
			else if(agent->browser == "Opera")
				browser = "Opera";
			else if(agent->browser == "Kindle")
				browser = "KindleBrowser";
			else if(!strncmp(agent->browser.c_str(), "Android", 7))
				browser = "AndroidBrowser";
			else
				browser = "UnknownBrowser";
			
			//Version
			version = agent->version;
		}

		//Add Platform
		agent_idents[platform] = 1;
		
		//Add Browser
		agent_idents[browser] = 1;
		
		//Add Browser Version
		agent_idents["Version"] = atoi(version.c_str());
		
		//Add Platform Type
		agent_idents[platform_type] = 1;
		
		agent->platform = platform;
		agent->platform_type = platform_type;
		agent->browser = browser;
		agent->version = version;
		
		return agent;
	}
};