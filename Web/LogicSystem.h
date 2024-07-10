#pragma once
#include "Singleton.h"
#include "thread"
#include "queue"
#include "MsgNode.h"
#include "memory"
#include "Connect.h"
#include "functional"
#include "map"
#include "JsonHelper.h"
#include "HttpConnect.h"
class LogicNode;
typedef std::function<void( std::shared_ptr<LogicNode>)> LogicCallBack;
typedef std::function<void( std::shared_ptr<Connect>,std::string)> HttpLogicCallBack;
enum MsgType :short
{
	EchoTest = 1001

};
class LogicNode
{
	friend class LogicSystem;
public:
	LogicNode(std::shared_ptr<Connect> connect, std::shared_ptr<Json::Value> value);
	//~LogicNode();
	MsgType GetMsgID();
	std::string GetWholeJsonMsg();
	std::shared_ptr<Json::Value> GetJson();
	short GetWholeJsonLen();
	std::shared_ptr<Connect> GetConnect();
private :
	
	std::shared_ptr<Json::Value> v;
	std::shared_ptr<Connect> connect;
	short len;
};
class LogicSystem : public Singleton<LogicSystem>
{
	friend class Singleton<LogicSystem>;
public:
	void Start();
	/// <summary>
	/// 投递逻辑
	/// </summary>
	void PostLogic(std::shared_ptr<Connect> connect,char* jsonData, short len);
	void Stop();
	void ProcessHttpCallBack(std::shared_ptr<Connect> connect,const char* target);
private:
	LogicSystem();
	void ProcessLogicQueue();
	void ProcessSingleLogicNode();
	void RegisterMsgLogic();
	std::unique_ptr<std::thread> thread;//逻辑处理线程
	std::queue< std::shared_ptr<LogicNode>> logicQueue;
	std::mutex mut;
	bool ifStop;
	std::condition_variable consum;
	std::map<MsgType,LogicCallBack> logicTable;
	std::map<std::string ,HttpLogicCallBack> httpLogicTable;
};

