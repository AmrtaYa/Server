#include "DbManager.h"
#include "LogManager.h"
#include "SingleDB.h"
DbManager::~DbManager()
{
}
DbManager::DbManager()
{
}
void DbManager::Init(DBConfig* config, short dbNum)
{
	for (size_t i = 0; i < dbNum; ++i)
	{
		DBConfig* c = &config[i];
		auto db = CreateSingleDB(c);
		db->Start();
		dbMap.insert(std::make_pair(c->dbType,db));
	}
}

SingleDB& DbManager::GetDB(DataBaseType dbType)
{
	auto db = dbMap[dbType];
	return *db.get();
}

std::shared_ptr<SingleDB> DbManager::CreateSingleDB(DBConfig* config)
{
	std::shared_ptr<SingleDB> db;
	switch (config->dbType)
	{
	case Mongo:
		 db = std::make_shared<SingleDB>(config);
		 break;
	default:
		LogManager::Log("²»´æÔÚ"+config->dbType);
		break;
	}
	return db;
}

DBConfig::DBConfig(std::string ip, short port, DataBaseType dbType):
	ip(ip),port(port),dbType(dbType)
{
}
