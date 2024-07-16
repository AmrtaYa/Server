#pragma once
#include "string"
namespace Entity
{
	struct  DBEntity
	{
	public:
		long ID;
	};


	struct ServerInfo : public DBEntity
	{
	public:
		std::string version;
		std::string sqlName;

	};
	struct UserInfo : public DBEntity
	{
	public:
		std::string account;
		std::string pwd;
	};
}