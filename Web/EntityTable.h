#pragma once
class DbManager;
#include "DBEntity.h"
#include "json/value.h"
#include "Windows.h"
#include <sqlext.h>
class EntityTable
{
	friend class DbManager;
private:
	static Entity::ServerInfo* ServerInfoProcess(SQLHSTMT hstmt);
	static Entity::UserInfo* UserInfoProcess(SQLHSTMT hstmt);
};

