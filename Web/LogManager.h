#pragma once
#include <string>

//��ӡ��־
static class LogManager
{
public:
	 LogManager();
	 ~LogManager();
	 static void Log(std::string msg);
private :
	static tm* p;
	static time_t* timep;
};

