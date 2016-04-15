#include <sstream>

struct AgentInfo
{
	std::string platform;
	std::string browser;
	std::string version;
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
	
		std::string cmd = "C:/PHP5.6/php.exe C:/PHP5.6/user_agent.php \"" + ua + "\"";
	
		FILE *fp = popen(cmd.c_str(), "r");
		
		char buf[1024];
		char *result = fgets(buf, 1024, fp);
		pclose(fp);
		
		//Error
		if(result == NULL)
			return NULL;
		//Good
		else
		{
			std::string result_string = result;
			std::stringstream data(result_string);
			
			std::getline(data,agent->platform,':');
			std::getline(data,agent->browser,':');
			std::getline(data,agent->version,':');
			
			return agent;
		}
	}
};