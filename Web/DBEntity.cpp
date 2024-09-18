#include "DBEntity.h"
#include "Reflect.h"
#define DBREG_F(EntityName,FieldName)\
REGISTER_FIELD_NS(Entity,EntityName,FieldName);
#define DBREG(EntityName) \
REGISTER_CLASS_NS(Entity,EntityName);\
DBREG_F(EntityName,ID);

//dbentity
DBREG(DBEntity);

//ServerInfoEntity
DBREG(ServerInfo);
DBREG_F(ServerInfo, version);
DBREG_F(ServerInfo, sqlName);
//userinfoEntity
DBREG(UserInfo);
DBREG_F(UserInfo, account);
DBREG_F(UserInfo, pwd);