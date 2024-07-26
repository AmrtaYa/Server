#pragma once
#include "string"

namespace Entity
{

	struct  DBEntity
	{
	public:
		long long ID;
	};

	struct ServerInfo : public DBEntity
	{
	public:
		char version[200];
		char sqlName[50];

	};
	struct UserInfo : public DBEntity
	{
	public:
		char account[50];
		char pwd[50];

	};

}
