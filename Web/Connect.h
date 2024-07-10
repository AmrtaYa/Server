#pragma once
#include "BoostDefine.h"
#include "boost/asio.hpp"
#include "memory"
#include "MsgNode.h"
/// <summary>
/// 连接基类
/// </summary>
class Connect :public std::enable_shared_from_this<Connect>
{
public:
	Connect(IO_Context& io,std::string uuid);
	virtual void Start();
	virtual void Close();
	std::string GetID();
	Tcp::socket& GetSocket();
	virtual void Write(SendNode* node);
protected:
	std::string uuid;
	std::shared_ptr<Tcp::socket> socket;
	IO_Context& io;
};

