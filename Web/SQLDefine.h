#pragma once
#include "DBEntity.h"
#include "vector"
#include "string"
#define SQLMAXBUFFER 200 //SQL��������ֽ�
typedef std::function<Entity::DBEntity**(SQLHSTMT hstmt,bool IfSingle)> EntityFindFunc;//Entity���һ��{����
typedef std::function<std::string(SQLHSTMT hstmt,Entity::DBEntity* entity ,bool IfSingle)> EntityUpdateFunc;//Entity���»��{����