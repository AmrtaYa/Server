#include "EntityTable.h"
#include "LogManager.h"
#include "iostream"
#include "sstream"
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
