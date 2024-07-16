#include "SQLServer.h"
#include "DbManager.h"
#include "iostream"
#include "LogManager.h"
#include "SQLDefine.h"
#include "SQLServer.h"
#include "SingleDB.h"
SQLServer::SQLServer(DBConfig* config) : SingleDB(config), henv(SQL_NULL_HENV), hdbc1(SQL_NULL_HDBC), hstmt1(SQL_NULL_HSTMT)
{
	dataName = config->szDSN;
	userID = config->userID;
	pwd = config->pwd;
}

void SQLServer::Start()
{
	try
	{
	sqlHandle = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	sqlHandle = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);

	sqlHandle = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc1);
	sqlHandle = SQLConnect(hdbc1, dataName, SQL_NTS, userID, SQL_NTS, pwd, SQL_NTS);
	if ((sqlHandle != SQL_SUCCESS) && (sqlHandle != SQL_SUCCESS_WITH_INFO))
	{
		std::string serverStr("SQLServer数据库连接失败");
		serverStr += ("          " + config->ip + ":" + std::to_string(config->port));
		LogManager::Log(serverStr);
		Stop();
		return;
	}
	std::string serverStr("SQLServer数据库连接成功");
	serverStr += ('\t' + config->ip + ":" + std::to_string(config->port));
	LogManager::Log(serverStr);
	}
	catch (const std::exception& e)
	{
		std::string serverStr("SQLServer数据库连接失败");
		serverStr += ("          " + config->ip + ":" + std::to_string(config->port)+"\t");
		serverStr += e.what();
		LogManager::Log(serverStr);
		Stop();
	}
	
}

void SQLServer::Stop()
{
	SQLDisconnect(hdbc1);
	SQLFreeHandle(SQL_HANDLE_DBC,hdbc1);
	SQLFreeHandle(SQL_HANDLE_ENV,henv);
}

void SQLServer::OperateSQL(std::string operatorStr)
{
	sqlHandle = SQLAllocHandle(SQL_HANDLE_STMT, hdbc1, &hstmt1);
	WCHAR container[SQLMAXBUFFER];
	swprintf(container, operatorStr.size(), L"%hs", operatorStr);
	sqlHandle = SQLExecDirect(hstmt1,container, operatorStr.size());

	sqlHandle = SQLCloseCursor(hstmt1);
	sqlHandle = SQLFreeHandle(SQL_HANDLE_STMT,hstmt1);
}
