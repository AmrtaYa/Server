#pragma once
#include "string"
#include "memory"
class DBConfig;
class SingleDB
{
public:
	SingleDB(DBConfig* config);
	virtual void Start();
	virtual void Stop();
	//int Find();
	//int Insert();
	//int Delte();
	//int Update();
	//int OperateSQL();
protected:
	std::shared_ptr<DBConfig> config;
};

