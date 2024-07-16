#pragma once
#include "Singleton.h"
#include "vector"
#include <map>
#include "memory"
#include "SQLServer.h"
#include "DBEntity.h"
#include "functional"
#include "json/value.h"

#include "SingleDB.h"
#include "SQLServer.h"

enum DataBaseType
{
	MongoType,
	SQLServerType,
};
class DBConfig
{
public:
	DBConfig(std::string ip,short port,DataBaseType dbType);
	std::string ip;
	short port;
	DataBaseType dbType;
	WCHAR szDSN[30];
	WCHAR  userID[30];
	WCHAR  pwd[30];
};
/// <summary>
/// 数据库管理器
/// </summary>
class DbManager :public Singleton<DbManager>
{
	friend class  Singleton<DbManager>;
public:
	~DbManager();
	DbManager();
	void Init(DBConfig* config, short size);
	SingleDB& GetDB(DataBaseType dbType);
	void SetCurrentDB(DataBaseType dbType);
	template<typename T>
	T Find(std::string str, std::string cols);
protected:
	template<typename T>
	EntityFunc& GetReflect();
private:
	DataBaseType currentDBType;
	void RegisterEntity();
	std::shared_ptr<SingleDB> CreateSingleDB(DBConfig* config);
	std::map<DataBaseType, std::shared_ptr<SingleDB>> dbMap;
	std::map<std::string, EntityFunc > entityMap;
};
template<typename T>
EntityFunc& DbManager::GetReflect()
{
	auto func = entityMap.find(typeid(T).name());
	if (func != entityMap.end())
		return func->second;
	return entityMap.begin()->second;
}
template<typename T>
T DbManager::Find(std::string str, std::string cols)
{
	SQLServer* sql = static_cast<SQLServer*>(&(this->GetDB(currentDBType)));
	return sql->Find<T>(str, cols,GetReflect<T>());
}
#define DB_FIND(WhereStr,ColChoose,T) DbManager::GetInstance()->Find<T>(WhereStr, ColChoose); ;