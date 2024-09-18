#include <iostream>

#include <boost/pfr.hpp>

#include "IOServerPool.h"
#include "thread"
#include "Server.h"
#include "vector"
#include "ServerManager.h"
#include "LogicSystem.h"

#include "DBEntity.h"
#include "SQLServer.h"
#include "DbManager.h"



std::vector<ServerData> InitServerData()
{
	std::vector<ServerData> datas;
	//datas.emplace_back("0.0.0.0",1145,TcpServerType);
	datas.emplace_back("0.0.0.0", 1146, HttpServerType);
	return datas;
}
std::vector<DBConfig> InitDbData()
{
	std::vector<DBConfig> datas;
	DBConfig config1(std::string("192.168.174.129"), 2145, SQLServerType);
	//,(WCHAR*)"AmrtaSQLServer", (WCHAR*)"sa", (WCHAR*)"Gjc20021120!"

	CHAR dsnCHAR[30] = "AmrtaSQLServer";
	swprintf(config1.szDSN, 30, L"%hs", dsnCHAR);

	CHAR userCHAR[30] = "sa";
	swprintf(config1.userID, 30, L"%hs", userCHAR);

	CHAR pwdCHAR[30] = "Gjc20021120!";
	swprintf(config1.pwd, 30, L"%hs", pwdCHAR);
	datas.push_back(config1);
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
		DbManager::GetInstance()->Init(dbDatas.data(), dbDatas.size());
		ServerManager::GetInstance()->Init(datas.data(), 1);

		auto e = DB_FIND("","*",Entity::ServerInfo);
		IOServerPool::GetInstance()->Run();
		return 0;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}
