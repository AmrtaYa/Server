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
	//���ֹͣ���ǾͲ�����Ͷ���߼�
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
	std::string path;//����  xxx/XXX.html   ��ô.html�������ͣ���ǰ����·��
	//������Ҫ��������һ��.����
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
	//��߿���д������ʾ
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
		//����ǿյģ����һ��������У���ô��Ҫ�̶߳�������Լ����
		while (logicQueue.empty() && !ifStop)
		{
			consum.wait(lock);
		}
		//���ͣ���ˣ���Ҫִ���������߼������˳�
		if (ifStop)
		{
			//����ʣ�µ��߼�
			while (!logicQueue.empty())
			{
				ProcessSingleLogicNode();
			}
			return;
		}
		//���ûͣ�����߼�������Ҳ�ж��У���ô���д���
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
		LogManager::Log("δ�ҵ����Ӧ�ĺ�������ID :" + msgId);
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

	//���ݿ���֤��Ϣ
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
 /// ��������json���ȣ����ǵ�����Ϣ��������Ϣ����getmsg��str���ҵ�
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
