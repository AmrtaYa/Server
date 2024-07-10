#pragma once
#include "SingleDB.h"
class DBConfig;
class MongoDB :public  SingleDB
{
public:
	MongoDB(DBConfig* config);
	void Start() override;
	void Stop() override;
private:

};

