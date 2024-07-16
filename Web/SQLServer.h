#pragma once
#include "SingleDB.h"
#include "SQLDefine.h"
#include "DBEntity.h"
#include "sstream"
#include "CharHelper.h"
class DBConfig;
class SqlServerConfig;
class SQLServer: public SingleDB
{
public:
	SQLServer(DBConfig* config);
	 void Start() override;
	 void Stop() override;
	 void OperateSQL(std::string operatorStr) override;
	 template<typename T>
	 T Find(std::string str="", std::string cols = "*", EntityFunc& reflectFunc=NULL);
private:
	RETCODE sqlHandle;
	SQLHENV henv;
	SQLHDBC hdbc1;
	SQLHSTMT hstmt1;
};

/// <summary>
/// 
/// </summary>
/// <typeparam name="T">必须继承于DBEntity</typeparam>
/// <param name="str"></param>
/// <param name="cols"></param>
/// <returns></returns>
template<typename T>
T SQLServer::Find(std::string str, std::string cols, EntityFunc& reflectFunc)
{
	static_assert(std::is_base_of<Entity::DBEntity, T>::value, "泛型需要继承于DBEntity");
	std::stringstream sstream;
	std::string typeName(typeid(T).name());
	int allLen = typeName.length();
	int classNameFirst = typeName.find_first_of(":") + 2;
	typeName = typeName.substr(classNameFirst,allLen-classNameFirst);
	sstream <<"select " << cols <<" from " << typeName;
	if (!str.empty())
		sstream << " where " << str;
	sqlHandle = SQLAllocHandle(SQL_HANDLE_STMT, hdbc1, &hstmt1);

	//str拼接转换
	std::string operaStr;
	operaStr =sstream.str();
	size_t operaLen = operaStr.length();

	//WCHAR转换
	WCHAR* containerPtr=To_SQLWCHAR(operaStr);
	WCHAR container[SQLMAXBUFFER] ;
	wcscpy_s(container,containerPtr);

	sqlHandle = SQLExecDirect(hstmt1, container, operaLen);

	//查找相对应的转换实体函数
	auto e = reflectFunc(hstmt1);
	auto entityPtr=static_cast<T*>(e);
	auto entity = *entityPtr;

	delete entityPtr;
	sqlHandle = SQLCloseCursor(hstmt1);
	sqlHandle = SQLFreeHandle(SQL_HANDLE_STMT, hstmt1);
	return entity;
}


