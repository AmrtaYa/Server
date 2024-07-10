#include "LogicSystem.h"
#include "LogManager.h"
#include "LogicProcessTable.h"
#include "sstream"
void LogicSystem::Start()
{
	ifStop = false;
}

void LogicSystem::PostLogic(std::shared_ptr<Connect> connect, char* jsonData,short len)
{
	//如果停止了那就不进行投递逻辑
	std::unique_lock<std::mutex> lock(mut);
	if (ifStop)return;
	std::shared_ptr<Json::Value> value =JsonHelper::GetInstance()->CharToValue(jsonData,len) ;
	auto logicNode = std::make_shared<LogicNode>(connect, value);
	logicQueue.push(logicNode);
	if (logicQueue.size() > 0)
	{
		consum.notify_one();
	}
}



void LogicSystem::Stop()
{
	ifStop = true;
}

void LogicSystem::ProcessHttpCallBack(std::shared_ptr<Connect> connect,const char* target)
{
	std::string tarStr(target);
	std::stringstream ss(tarStr);
	std::string index;
	std::string type;
	std::string path;//例如  xxx/XXX.html   那么.html就是类型，而前面是路径
	//这里需要解决是最后一个.才行
	while (std::getline(ss, index, '.'))
	{
		path += index;
		path += '.';
	}
	std::string removeStr("." + index + ".");
	path.erase(path.length()- removeStr.length());
	
	type = index;
	//------------------------------------------------------------------------
	auto logic = httpLogicTable.find(std::string(type));
	if (logic != httpLogicTable.end())
	{
		auto callBack = logic->second;
		callBack(connect,std::string(path));
		return;
	}
	//这边可以写报错提示
}

LogicSystem::LogicSystem() 
{
	RegisterMsgLogic();
	thread = std::make_unique<std::thread>([this]() {
		this->ProcessLogicQueue();
		});
	
}

void LogicSystem::ProcessLogicQueue()
{
	std::unique_lock<std::mutex> lock(mut);
	while (true)
	{
		//如果是空的，并且还在运行中，那么需要线程堵塞，节约性能
		while (logicQueue.empty() && !ifStop)
		{
			consum.wait(lock);
		}
		//如果停服了，需要执行完所有逻辑才能退出
		if (ifStop)
		{
			//处理剩下的逻辑
			while (!logicQueue.empty())
			{
				ProcessSingleLogicNode();
			}
			return;
		}
		//如果没停服，逻辑队列中也有队列，那么进行处理
		ProcessSingleLogicNode();
	}
}

void LogicSystem::ProcessSingleLogicNode()
{
	auto logicData = logicQueue.front();
	MsgType msgId = logicData->GetMsgID();
	auto logicFunc = logicTable.find(msgId);
	if (logicFunc != logicTable.end())
		logicTable[msgId](logicData);
	else
		LogManager::Log("未找到相对应的函数处理ID :" + msgId);
	logicQueue.pop();
}

void LogicSystem::RegisterMsgLogic()
{
	logicTable.insert(std::make_pair<MsgType,LogicCallBack>(EchoTest,LogicProcessTable::TCP_ProcessEchoTest_1001));

	//Http
	httpLogicTable.insert(std::make_pair<std::string,HttpLogicCallBack>(std::string("html"), HttpLogicPorcessTable::Http_ProcessHtml));
	httpLogicTable.insert(std::make_pair<std::string,HttpLogicCallBack>(std::string("js"), HttpLogicPorcessTable::Http_ProcessJs));
	httpLogicTable.insert(std::make_pair<std::string,HttpLogicCallBack>(std::string("jpg"), HttpLogicPorcessTable::Http_ProcessJPGImg));
	httpLogicTable.insert(std::make_pair<std::string,HttpLogicCallBack>(std::string("png"), HttpLogicPorcessTable::Http_ProcessPNGImg));
	httpLogicTable.insert(std::make_pair<std::string,HttpLogicCallBack>(std::string("css"), HttpLogicPorcessTable::Http_ProcessCSS));

	//数据库验证信息
	httpLogicTable.insert(std::make_pair<std::string,HttpLogicCallBack>(std::string("confirm"), HttpLogicPorcessTable::Http_ProcessDBConfirm));
}

LogicNode::LogicNode(std::shared_ptr<Connect> connect, std::shared_ptr<Json::Value> value):
	len(value->size()),connect(connect),v(value)
{
	
}



MsgType LogicNode::GetMsgID()
{
	auto jsonValue = *v.get();
	MsgType msg = (MsgType)jsonValue["id"].asInt();
	return msg;
}

 std::string LogicNode::GetWholeJsonMsg()
{
	auto jsonValue = *v.get();
	auto m = jsonValue.toStyledString();
	return m;
}
 std::shared_ptr<Json::Value> LogicNode::GetJson()
 {
	 return v;
 }
 /// <summary>
 /// 返回整个json长度，不是单个信息，单个信息请用getmsg的str来找到
 /// </summary>
 /// <returns></returns>
 short LogicNode::GetWholeJsonLen()
 {
	 return GetWholeJsonMsg().length();
 }

 std::shared_ptr<Connect> LogicNode::GetConnect()
 {
	 return connect;
 }
