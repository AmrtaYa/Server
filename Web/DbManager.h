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
	DBConfig(std::string ip, short port, DataBaseType dbType);
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
	std::vector<T>  Find(std::string str, std::string cols);
	template<typename T>
	T  FindOne(std::string str, std::string cols);
	template<typename T>
	int UpdateOne(T entity);
	template<typename T>
	void CreateTable();
	void SQLOperator(std::string sqlStr);
protected:
	EntityFindFunc& GetReflect_Find();
	EntityUpdateFunc& GetReflect_Update();
private:
	DataBaseType currentDBType;
	void RegisterEntity();
	std::shared_ptr<SingleDB> CreateSingleDB(DBConfig* config);
	std::map<DataBaseType, std::shared_ptr<SingleDB>> dbMap;
	EntityFindFunc FindFunc;
	EntityUpdateFunc UpdateFunc;
};


template<typename T>
std::vector<T> DbManager::Find(std::string str, std::string cols)
{
	switch (currentDBType)
	{
	case MongoType:
		break;
	case SQLServerType:
	{
		SQLServer* sql = static_cast<SQLServer*>(&(this->GetDB(currentDBType)));
		return sql->Find<T>(str, cols, GetReflect_Find());
	}
	default:
		break;
	}
	return std::vector<T>();
}
template<typename T>
T DbManager::FindOne(std::string str, std::string cols)
{
	switch (currentDBType)
	{
	case MongoType:
		break;
	case SQLServerType:
	{
		SQLServer* sql = static_cast<SQLServer*>(&(this->GetDB(currentDBType)));
		return sql->FindOne<T>(str, cols, GetReflect_Find());
	}
	default:
		throw "DBType不存在";
		break;
	}
	return T();
}
template<typename T>
int DbManager::UpdateOne(T entity)
{
	switch (currentDBType)
	{
	case MongoType:
		break;
	case SQLServerType:
	{
		SQLServer* sql = static_cast<SQLServer*>(&(this->GetDB(currentDBType)));
		return sql->UpdateOne<T>(entity, GetReflect_Update());
	}
	default:
		throw "DBType不存在";
		break;
	}
	return -999;


}
template<typename T>
void DbManager::CreateTable()
{
	auto entity = DB_FIND("", "*", T);

}

#define DB_SQL(SQLStr) DbManager::GetInstance()->SQLOperator(SQLStr);
#define DB_FIND(WhereStr,ColChoose,T) DbManager::GetInstance()->Find<T>(WhereStr, ColChoose); 
#define DB_FIND_ONE(WhereStr,ColChoose,T) DbManager::GetInstance()->FindOne<T>(WhereStr, ColChoose); 
#define DB_UPDATE_ONE(Entity) DbManager::GetInstance()->UpdateOne(Entity);

