#pragma once
#include "DBEntity.h"
#include "vector"
#include "string"
#define SQLMAXBUFFER 200 //SQL操作最大字节
typedef std::function<Entity::DBEntity**(std::string typeName,SQLHSTMT hstmt,bool IfSingle)> EntityFindFunc;//Entity查找回調函数
typedef std::function<std::string(std::string typeName,SQLHSTMT hstmt,Entity::DBEntity* entity ,bool IfSingle)> EntityUpdateFunc;//Entity更新回調函数