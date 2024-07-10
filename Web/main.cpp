#include <iostream>
#include "IOServerPool.h"
#include "thread"
#include "Server.h"
#include "vector"
#include "ServerManager.h"
#include "LogicSystem.h"
#include "DbManager.h"
std::vector<ServerData> InitServerData()
{
	std::vector<ServerData> datas;
	datas.emplace_back("0.0.0.0",1145,TcpServerType);
	//datas.emplace_back("0.0.0.0",1146,HttpServerType);
	return datas;
}
std::vector<DBConfig> InitDbData()
{
	std::vector<DBConfig> datas;
	datas.emplace_back("0.0.0.0",27017,Mongo);
	return datas;
}
int main()
{
	try
	{
		int threadNum = std::thread::hardware_concurrency();
		auto datas = InitServerData();
		auto dbDatas = InitDbData();

		LogicSystem::GetInstance()->Start();
		IOServerPool::GetInstance()->InitPool(threadNum);
		DbManager::GetInstance()->Init(dbDatas.data(),1);
		ServerManager::GetInstance()->Init(datas.data(), 1);
		IOServerPool::GetInstance()->Run();
		return 0;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}
