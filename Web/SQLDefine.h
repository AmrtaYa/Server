#pragma once
#include "DBEntity.h"
#include "vector"
#include "string"
#define SQLMAXBUFFER 200 //SQL��������ֽ�
typedef std::function<Entity::DBEntity**(std::string typeName,SQLHSTMT hstmt,bool IfSingle)> EntityFindFunc;//Entity���һ��{����
typedef std::function<std::string(std::string typeName,SQLHSTMT hstmt,Entity::DBEntity* entity ,bool IfSingle)> EntityUpdateFunc;//Entity���»��{����