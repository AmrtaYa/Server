#pragma once
#include "Singleton.h"
#include "vector"
#include "SingleDB.h"
#include <map>
#include "memory"
enum DataBaseType
{
	Mongo,
	SQLServer,
};
class DBConfig
{
public:
	DBConfig(std::string ip,short port,DataBaseType dbType);
	std::string ip;
	short port;
	DataBaseType dbType;
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
	void Init(DBConfig* config,short dbNum);
	SingleDB& GetDB(DataBaseType dbType);
private:
	std::shared_ptr<SingleDB> CreateSingleDB(DBConfig* config);
	std::map<DataBaseType, std::shared_ptr<SingleDB>> dbMap;
};

