#include "EntityTable.h"


Entity::ServerInfo* EntityTable::ServerInfoProcess(SQLHSTMT hstmt)
{
    Entity::ServerInfo* info = new Entity::ServerInfo();
    SQLLEN len = SQL_NTS;

    char ver[200];
    char name[50];
    ZeroMemory(ver, 200);
    ZeroMemory(name, 50);
    SQLBindCol(hstmt,1,SQL_C_CHAR, ver,200, &len);
    SQLBindCol(hstmt,2,SQL_C_CHAR, name,50, &len);
    auto ret = SQLFetch(hstmt);
    while (ret!= SQL_NO_DATA)
    {
         ret = SQLFetch(hstmt);
    }
    info->version = ver;
    info->sqlName = name;
    return info;
}

Entity::UserInfo* EntityTable::UserInfoProcess(SQLHSTMT hstmt)
{
    Entity::UserInfo* info = new Entity::UserInfo();
    SQLLEN len = SQL_NTS;

    char user[50];
    char pwd[50];
    ZeroMemory(user, 50);
    ZeroMemory(pwd, 50);
    SQLBindCol(hstmt, 1, SQL_C_LONG, &info->ID, 0, &len);
    SQLBindCol(hstmt, 2, SQL_C_CHAR, user, 50, &len);
    SQLBindCol(hstmt, 3, SQL_C_CHAR, pwd, 50, &len);
    auto ret = SQLFetch(hstmt);
    while (ret != SQL_NO_DATA)
    {
        ret = SQLFetch(hstmt);
    }
    info->account = user;
    info->pwd = pwd;
    return info;
}
