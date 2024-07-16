#pragma once
#include "DBEntity.h"
#define SQLMAXBUFFER 200 //SQL操作最大字节
typedef std::function<Entity::DBEntity*(SQLHSTMT hstmt)> EntityFunc;//Entity毁掉函数