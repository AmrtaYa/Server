#pragma once
#include "Singleton.h"
#include "json/json.h"
#include "json/value.h"
#include "json/reader.h"
#include "LogManager.h"

 class  JsonHelper :public Singleton<JsonHelper>
{
	 friend class Singleton<JsonHelper>;
private:
	 JsonHelper()
	{
		 reader = builder.newCharReader();
	}
public:
	~JsonHelper()
	{

	}
	 std::shared_ptr<Json::Value> CharToValue(char* data, short len)
	{

		std::shared_ptr<Json::Value> v = std::make_shared<Json::Value>();
		std::string err;
		reader->parse(data, data + len, v.get(), &err);
		if (!err.empty())
		{
			LogManager::Log(err);
		}
		return v;
	}

private:
	 Json::CharReaderBuilder builder;
	 Json::CharReader* reader;
};

