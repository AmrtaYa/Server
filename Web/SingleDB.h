#pragma once
#include "string"
#include "memory"
#include "Windows.h"
#include "sql.h"
#include "sqlext.h"
#include <sqltypes.h>
#include <odbcss.h>
#include <functional>
#include "SQLDefine.h"
class DBConfig;
class SingleDB
{
public:
	SingleDB(DBConfig* config);
	virtual void Start();
	virtual void Stop();
	template<typename T>
	 T Find(std::string str, std::string cols, EntityFunc& reflectFunc);
	//int Insert();
	//int Delte();
	//int Update();

	virtual void OperateSQL(std::string operatorStr);
protected:
	std::shared_ptr<DBConfig> config;
	SQLWCHAR* dataName;
	SQLWCHAR* userID;
	SQLWCHAR* pwd;
};
/// <summary>
/// 
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="str">筛选条件</param>
/// <param name="cols">列的筛选条件</param>
/// <returns></returns>
template<typename T>
 T SingleDB::Find(std::string str, std::string cols, EntityFunc& reflectFunc)
{
	/*SQLServer* sql = static_cast<SQLServer*>(this);
	
	return sql->Find();*/
	 return T();
}

