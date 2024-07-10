#pragma once
#include "MsgDefine.h"
#include "iostream"
#include "json/reader.h"
#include "json/value.h"
#include "boost/asio.hpp"
class BaseNode
{
public:
	BaseNode(size_t size)
		: size(size), data(new char[size])
	{}
	~BaseNode()
	{
		delete[] data;
	}
	char* GetData()
	{
		return data;
	}
	void Clear()
	{
		memset(data, 0, size);
	}
protected:
	char* data;
	size_t size;

};
class  HeadMsgNode :public BaseNode
{
public:
	HeadMsgNode() :BaseNode(MSGHEAD)
	{
	}
	short GetMsgLen()
	{
		short len = 0;
		memcpy(&len, GetData(), MSGHEAD);
		len = boost::asio::detail::socket_ops::network_to_host_short(len);
		return len;
	}

private:

};

class  MsgNode :public BaseNode
{
public:
	MsgNode() :BaseNode(MAXBUFFER), msgIndex(new char[MAXBUFFER])
	{
	}
	~MsgNode()
	{
		delete[] msgIndex;
	}
	char* GetMsg(short len)
	{
		memset(msgIndex, 0, MAXBUFFER);
		memcpy(msgIndex, GetData(), len);
		return msgIndex;
	}
private:
	char* msgIndex;
};

class SendNode : public BaseNode
{
public:
	SendNode(short len,const char* msg) :BaseNode(len + MSGHEAD),len(len+ MSGHEAD)
	{
		auto netLen = boost::asio::detail::socket_ops::host_to_network_short(len);
		memcpy(data,&netLen, MSGHEAD);
		memcpy(data+ MSGHEAD,msg,len);
	}
	short GetLen()
	{
		return len;
	}
	~SendNode()
	{
	}

private:
	short len;
};