#include "LogManager.h"
#include "iostream"
#include "time.h"
tm* LogManager::p = new tm();
time_t* LogManager::timep = new time_t();
LogManager::LogManager()
{

}
LogManager::~LogManager()
{
	if (p != nullptr)
		delete p;
	if (timep != nullptr)
	delete timep;
}

void LogManager::Log(std::string msg)
{
	time(timep);
	auto time = localtime_s(p, timep);
	int year = p->tm_year + 1900;
	int month = p->tm_mon + 1;
	int day = p->tm_mday;
	int hour = p->tm_hour;
	int min = p->tm_min;
	int sec = p->tm_sec;
	std::cout << year << '/' <<
		month << '/' <<
		day << ':' <<
		hour << ':' <<
		min << ':' <<
		sec << '\t'
		<< msg << std::endl;

}
