#pragma once
#include "DBEntity.h"
#define SQLMAXBUFFER 200 //SQL��������ֽ�
typedef std::function<Entity::DBEntity*(SQLHSTMT hstmt)> EntityFunc;//Entity�ٵ�����