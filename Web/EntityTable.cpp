#include "EntityTable.h"
#include "LogManager.h"
#include "iostream"
#include "sstream"
#include "Reflect.h"
#include "vector"
/// <summary>
/// 字段名字转换
/// </summary>
/// <param name="fieldName"></param>
/// <returns></returns>
int TurnSQLType(std::string typeName)
{
	int t = 0;
	
	if (typeName.find("long") != std::string::npos || typeName.find("__int64") != std::string::npos)
		t = SQL_C_LONG;
	else if (typeName.find("char")!=std::string::npos)
		t = SQL_C_CHAR;

	return t;
}
DBEntity** EntityTable::Entity_Find(std::string typeName, SQLHSTMT hstmt, bool IfSingle)
{
	//将typeid的class字符串去除
	auto spacePos = typeName.find_first_of(' ');
	typeName = typeName.substr(spacePos + 1);
	auto entityType = REFLECT_GET_TYPE(typeName);
	auto sizeLen = entityType.Size() * MAXQUEUE;
	char** info = new char* [sizeLen];
	SQLLEN len = SQL_NTS;
	int index = 0;
	auto fields = REFLECT_ALLFIELDS(typeName);
	void** fieldInstance = new void* [fields.size()];
	//注册这个实体里的每一个字段并且输入数据
	for (size_t i = 0; i < fields.size(); ++i)
	{
		auto fieldName = fields[i].GetFieldName();
		auto instance = fields[i].CreateInstance();
		fieldInstance[i] = instance;
		SQLBindCol(hstmt, i + 1, TurnSQLType(fields[i].GetType()), instance, fields[i].len(), &len);
	}
	auto ret = SQLFetch(hstmt);
	char* single = new char[entityType.Size()];
	//设置ID,因为ID永远在最前面
	memset(single, EmptyEntity, sizeof(EmptyEntity));
	for (size_t i = 0; i < fields.size(); ++i)
	{
		//single做偏移，传入每一个实例里的数据以及长度(长度与实例里顺序一致)
		memcpy(single + fields[i].GetOffset(), fieldInstance[i], fields[i].len());
	}
	info[index] = single;
	index++;
	if (IfSingle)
		return (DBEntity**)info;
	while (ret != SQL_NO_DATA)
	{
		for (size_t i = 0; i < fields.size(); ++i)
		{
			ZeroMemory(fieldInstance[i], fields[i].len());
		}
		memset(fieldInstance[0], EmptyEntity, sizeof(EmptyEntity));
		for (size_t i = 0; i < fields.size(); ++i)
		{
			auto fieldName = fields[i].GetFieldName();
			SQLBindCol(hstmt, i + 1, TurnSQLType(fields[i].GetType()), fieldInstance[i], fields[i].len(), &len);
		}
		auto ret = SQLFetch(hstmt);
		char* single = new char[entityType.Size()];
		//设置ID,因为ID永远在最前面
		memset(single, EmptyEntity, sizeof(EmptyEntity));
		for (size_t i = 0; i < fields.size(); ++i)
		{
			//single做偏移，传入每一个实例里的数据以及长度(长度与实例里顺序一致)
			memcpy(single + fields[i].GetOffset(), fieldInstance[i], fields[i].len());
		}
		info[index] = single;
		index++;
	}

	return (DBEntity**)info;
}
DBEntity** EntityTable::ServerInfoProcess_Find(SQLHSTMT hstmt, bool IfSingle)
{
	ServerInfo** info = new ServerInfo * [MAXQUEUE];
	SQLLEN len = SQL_NTS;
	char version[200];
	char sqlName[50];

	int index = 0;
	long ID = EmptyEntity;
	ZeroMemory(version, 200);
	ZeroMemory(sqlName, 50);
	SQLBindCol(hstmt, 1, SQL_C_LONG, &ID, 0, &len);
	SQLBindCol(hstmt, 2, SQL_C_CHAR, version, 200, &len);
	SQLBindCol(hstmt, 3, SQL_C_CHAR, sqlName, 50, &len);
	auto ret = SQLFetch(hstmt);
	ServerInfo* single = new ServerInfo();
	single->ID = ID;
	memcpy(single->version, version, 200);
	memcpy(single->sqlName, sqlName, 50);
	info[index] = single;
	index++;
	if (IfSingle)
		return (DBEntity**)info;
	while (ret != SQL_NO_DATA)
	{
		ID = EmptyEntity;
		ZeroMemory(version, 200);
		ZeroMemory(sqlName, 50);
		SQLBindCol(hstmt, 1, SQL_C_LONG, &ID, 0, &len);
		SQLBindCol(hstmt, 2, SQL_C_CHAR, version, 200, &len);
		SQLBindCol(hstmt, 3, SQL_C_CHAR, sqlName, 50, &len);
		ret = SQLFetch(hstmt);
		ServerInfo* single = new ServerInfo();
		single->ID = ID;
		memcpy(single->version, version, 200);
		memcpy(single->sqlName, sqlName, 50);
		info[index] = single;
		index++;
	}
	return (DBEntity**)info;
}

DBEntity** EntityTable::UserInfoProcess_Find(SQLHSTMT hstmt, bool IfSingle)
{
	UserInfo** info = new UserInfo * [MAXQUEUE];
	SQLLEN len = SQL_NTS;
	char user[50];
	char pwd[50];

	int index = 0;
	auto typeLen = sizeof(UserInfo);
	long ID = EmptyEntity;
	ZeroMemory(user, 50);
	ZeroMemory(pwd, 50);
	SQLBindCol(hstmt, 1, SQL_C_LONG, &ID, 0, &len);
	SQLBindCol(hstmt, 2, SQL_C_CHAR, user, 50, &len);
	SQLBindCol(hstmt, 3, SQL_C_CHAR, pwd, 50, &len);
	auto ret = SQLFetch(hstmt);
	UserInfo* single = new UserInfo();
	single->ID = ID;
	memcpy(single->account, user, 50);
	memcpy(single->pwd, pwd, 50);
	info[index] = single;
	index++;
	if (IfSingle)
		return (DBEntity**)info;
	while (ret != SQL_NO_DATA)
	{
		ID = EmptyEntity;
		ZeroMemory(user, 50);
		ZeroMemory(pwd, 50);
		SQLBindCol(hstmt, 1, SQL_C_LONG, &ID, 0, &len);
		SQLBindCol(hstmt, 2, SQL_C_CHAR, user, 50, &len);
		SQLBindCol(hstmt, 3, SQL_C_CHAR, pwd, 50, &len);
		ret = SQLFetch(hstmt);
		UserInfo* single = new UserInfo();
		single->ID = ID;
		memcpy(single->account, user, 50);
		memcpy(single->pwd, pwd, 50);
		info[index] = single;
		index++;
	}
	return (DBEntity**)info;
}

std::string EntityTable::Entity_Update(std::string typeName, SQLHSTMT hstmt, Entity::DBEntity* entity, bool IfSingle)
{
	return std::string();
}

std::string EntityTable::ServerInfoProcess_Update(SQLHSTMT hstmt, Entity::DBEntity* entity, bool IfSingle)
{
	ServerInfo* eP = static_cast<ServerInfo*>(entity);
	ServerInfo e = *eP;
	std::stringstream os;
	os << "ID = " << e.ID << ", version = " << e.version << ", sqlName = " << e.sqlName << " ";
	return os.str();
}

std::string EntityTable::UserInfoProcess_Update(SQLHSTMT hstmt, Entity::DBEntity* entity, bool IfSingle)
{
	UserInfo* eP = static_cast<UserInfo*>(entity);
	UserInfo e = *eP;
	std::stringstream os;
	os << "ID = " << e.ID << ", account = " << e.account << ", pwd = " << e.pwd << " ";
	return os.str();
}
