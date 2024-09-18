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
	else if (typeName.find("char") != std::string::npos)
		t = SQL_C_CHAR;

	return t;
}
DBEntity** EntityTable::Entity_Find(std::string typeName, SQLHSTMT hstmt, bool IfSingle)
{
	//将typeid的class字符串去除
	auto spacePos = typeName.find_first_of(' ');
	typeName = typeName.substr(spacePos + 1);
	auto entityType = REFLECT_GET_TYPE(typeName);
	auto entitySize = entityType.Size();

	auto sizeLen = entitySize * MAXQUEUE;
	DBEntity** info = new DBEntity * [MAXQUEUE];

	SQLLEN len = SQL_NTS;
	int index = 0;
	auto fields = REFLECT_ALLFIELDS(typeName);

	//char** fieldInstance = new char* [fields.size() * 16];//按照最大16字节给  需要x个字段地址  记得把里面所有的地址内容释放
	std::vector<void*> fieldInstance;
	//注册这个实体里的每一个字段并且输入数据
	for (size_t i = 0; i < fields.size(); ++i)
	{
		auto fieldName = fields[i].GetFieldName();
		void* instance = fields[i].CreateInstance();
		fieldInstance.push_back(instance);
		SQLBindCol(hstmt, i + 1, TurnSQLType(fields[i].GetType()), instance, fields[i].len(), &len);
	}

	auto ret = SQLFetch(hstmt);

	char* single = new char[entitySize];
	//设置ID,因为ID永远在最前面
	memset(single, 0, entitySize);
	for (size_t i = 0; i < fields.size(); ++i)
	{
		//single做偏移，传入每一个实例里的数据以及长度(长度与实例里顺序一致)
		memcpy(single + fields[i].GetOffset(), fieldInstance[i], fields[i].len());
	}

	info[index] = (DBEntity*)single;
	index++;

	//回收cpy后的内存
	for (size_t i = 0; i < fieldInstance.size(); i++)
	{
		delete  fieldInstance[i];
	}

	if (IfSingle)
		return (DBEntity**)info;


	while (ret != SQL_NO_DATA)
	{
		//重置fieldInstance内的数据
		fieldInstance.clear();
		for (size_t i = 0; i < fields.size(); ++i)
		{
			auto fieldName = fields[i].GetFieldName();
			void* instance = fields[i].CreateInstance();
			fieldInstance.push_back(instance);
			SQLBindCol(hstmt, i + 1, TurnSQLType(fields[i].GetType()), instance, fields[i].len(), &len);
		}
		ret = SQLFetch(hstmt);

		char* single = new char[entitySize];
		//设置ID,因为ID永远在最前面
		memset(single,0,entitySize);
		
		for (size_t i = 0; i < fields.size(); ++i)
		{
			//single做偏移，传入每一个实例里的数据以及长度(长度与实例里顺序一致)
			memcpy(single + fields[i].GetOffset(), fieldInstance[i], fields[i].len());
		}

		info[index] = (DBEntity*)single;
		index++;
		if (index >= MAXQUEUE - 1)break;//防止溢出
		//回收cpy后的内存
		for (size_t i = 0; i < fieldInstance.size(); i++)
		{
			delete  fieldInstance[i];
		}
	}
	info[index - 1]->ID = EmptyEntity;
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
