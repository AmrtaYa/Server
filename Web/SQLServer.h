#pragma once
#include "SingleDB.h"
#include "SQLDefine.h"
#include "DBEntity.h"
#include "sstream"
#include "CharHelper.h"
#include "vector"
#include "EntityDefine.h"

class DBConfig;
class SqlServerConfig;

template<typename T>
class ArrInfo
{
public:
	T* arr;
	int len;
};
class SQLServer : public SingleDB
{
public:
	SQLServer(DBConfig* config);
	void Start() override;
	void Stop() override;
	void OperateSQL(std::string operatorStr) override;
	template<typename T>
	std::vector<T> Find(std::string str = "", std::string cols = "*", EntityFindFunc& reflectFunc=NULL);
	template<typename T>
	T FindOne(std::string str, std::string cols, EntityFindFunc& reflectFunc);
	template<typename T>
	int UpdateOne(T entity, EntityUpdateFunc& func);
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
std::vector<T> SQLServer::Find(std::string str, std::string cols, EntityFindFunc& reflectFunc)
{
	static_assert(std::is_base_of<Entity::DBEntity, T>::value, "泛型需要继承于DBEntity");
	std::stringstream sstream;
	std::string typeName(typeid(T).name());
	int allLen = typeName.length();
	int classNameFirst = typeName.find_first_of(":") + 2;
	typeName = typeName.substr(classNameFirst, allLen - classNameFirst);
	sstream << "select " << cols << " from " << typeName;
	if (!str.empty())
		sstream << " where " << str;
	sqlHandle = SQLAllocHandle(SQL_HANDLE_STMT, hdbc1, &hstmt1);

	//str拼接转换
	std::string operaStr;
	operaStr = sstream.str();
	size_t operaLen = operaStr.length();

	//WCHAR转换
	WCHAR* containerPtr = To_SQLWCHAR(operaStr);
	WCHAR container[SQLMAXBUFFER];
	wcscpy_s(container, containerPtr);

	sqlHandle = SQLExecDirect(hstmt1, container, operaLen);

	//查找相对应的转换实体函数
	Entity::DBEntity** eData = reflectFunc(std::string(typeid(T).name()),hstmt1, false);
	std::vector<T> entity;
	int forIndex = 0;
	while (true)
	{
		T* ePtr = static_cast<T*>(eData[forIndex]);
		T e = *ePtr;
		if (e.ID == EmptyEntity)
		{
			break;
		}
		entity.push_back(e);
		forIndex++;
	}
	free(eData);
	sqlHandle = SQLCloseCursor(hstmt1);
	sqlHandle = SQLFreeHandle(SQL_HANDLE_STMT, hstmt1);

	return entity;
}

template<typename T>
T SQLServer::FindOne(std::string str, std::string cols, EntityFindFunc& reflectFunc)
{
	static_assert(std::is_base_of<Entity::DBEntity, T>::value, "泛型需要继承于DBEntity");
	std::stringstream sstream;
	std::string typeName(typeid(T).name());
	int allLen = typeName.length();
	int classNameFirst = typeName.find_first_of(":") + 2;
	typeName = typeName.substr(classNameFirst, allLen - classNameFirst);
	sstream << "select " << cols << " from " << typeName;
	if (!str.empty())
		sstream << " where " << str;
	sqlHandle = SQLAllocHandle(SQL_HANDLE_STMT, hdbc1, &hstmt1);

	//str拼接转换
	std::string operaStr;
	operaStr = sstream.str();
	size_t operaLen = operaStr.length();

	//WCHAR转换
	WCHAR* containerPtr = To_SQLWCHAR(operaStr);
	WCHAR container[SQLMAXBUFFER];
	wcscpy_s(container, containerPtr);

	sqlHandle = SQLExecDirect(hstmt1, container, operaLen);

	//查找相对应的转换实体函数
	Entity::DBEntity** eData = reflectFunc(std::string(typeid(T).name()), hstmt1, true);
	T* ePtr = static_cast<T*>(eData[0]);
	T e = *ePtr;


	delete[] eData;
	sqlHandle = SQLCloseCursor(hstmt1);
	sqlHandle = SQLFreeHandle(SQL_HANDLE_STMT, hstmt1);

	return e;
}

template<typename T>
inline int SQLServer::UpdateOne(T entity, EntityUpdateFunc& func)
{
	static_assert(std::is_base_of<Entity::DBEntity, T>::value, "泛型需要继承于DBEntity");
	sqlHandle = SQLAllocHandle(SQL_HANDLE_STMT, hdbc1, &hstmt1);

	std::stringstream sstream;
	std::string typeName(typeid(T).name());
	int allLen = typeName.length();
	int classNameFirst = typeName.find_first_of(":") + 2;
	typeName = typeName.substr(classNameFirst, allLen - classNameFirst);
	std::string entitySql = func(std::string(typeid(T).name()),hstmt1, (Entity::DBEntity*)(&entity), true);
	sstream << "update " << typeName << " set " << entitySql << "where ID = "<<std::to_string(entity.ID);

	//str拼接转换
	std::string operaStr;
	operaStr = sstream.str();
	size_t operaLen = operaStr.length();

	//WCHAR转换
	WCHAR* containerPtr = To_SQLWCHAR(operaStr);
	WCHAR container[SQLMAXBUFFER];
	wcscpy_s(container, containerPtr);

	sqlHandle = SQLExecDirect(hstmt1, container, operaLen);

	sqlHandle = SQLCloseCursor(hstmt1);
	sqlHandle = SQLFreeHandle(SQL_HANDLE_STMT, hstmt1);
	return sqlHandle;
}


