#pragma once
class DbManager;
#include "DBEntity.h"
#include "json/value.h"
#include "Windows.h"
#include <sqlext.h>
#include "vector"
#include "EntityDefine.h"
using namespace Entity;
class EntityTable
{
	friend class DbManager;
private:
	static  DBEntity** ServerInfoProcess_Find(SQLHSTMT hstmt,bool IfSingle);
	static  DBEntity** UserInfoProcess_Find(SQLHSTMT hstmt, bool IfSingle);

	static std::string ServerInfoProcess_Update(SQLHSTMT hstmt, Entity::DBEntity* entity, bool IfSingle);
	static std::string UserInfoProcess_Update(SQLHSTMT hstmt, Entity::DBEntity* entity, bool IfSingle);

};

