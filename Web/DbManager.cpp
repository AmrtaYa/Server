#include "DbManager.h"
#include "LogManager.h"
#include "SingleDB.h"
#include"SQLServer.h"
#include "MongoDB.h"
#include "vector"
#include "EntityTable.h"
DbManager::~DbManager()
{
}
DbManager::DbManager()
{
}
void DbManager::Init(DBConfig* config, short size)
{
	for (size_t i = 0; i < size; ++i)
	{
		DBConfig* c = &config[i];
		auto db = CreateSingleDB(c);
		db->Start();
		dbMap.insert(std::make_pair(c->dbType, db));
	}
	RegisterEntity();
	SetCurrentDB(config[0].dbType);
}



SingleDB& DbManager::GetDB(DataBaseType dbType)
{
	auto db = dbMap[dbType];
	return  *db.get();
}

void DbManager::SetCurrentDB(DataBaseType dbType)
{
	this->currentDBType = dbType;
}

void DbManager::RegisterEntity()
{
	entityMap.emplace(std::make_pair(typeid(Entity::ServerInfo).name(), EntityTable::ServerInfoProcess));
	entityMap.emplace(std::make_pair(typeid(Entity::UserInfo).name(), EntityTable::UserInfoProcess));
}

std::shared_ptr<SingleDB> DbManager::CreateSingleDB(DBConfig* config)
{
	std::shared_ptr<SingleDB> db = nullptr;
	switch (config->dbType)
	{
	case MongoType:
		db = std::make_shared<MongoDB>(config);
		break;
	case SQLServerType:
	{
		db = std::make_shared<SQLServer>(config);
		break;
	}
	default:
		LogManager::Log("������" + config->dbType);
		break;
	}
	return db;
}

DBConfig::DBConfig(std::string ip, short port, DataBaseType dbType) :
	ip(ip), port(port), dbType(dbType)
{
}


