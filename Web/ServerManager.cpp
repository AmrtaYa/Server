#include "ServerManager.h"
#include "iostream"
#include "TcpServer.h"
#include "IOServerPool.h"
#include "HttpServer.h"
ServerManager::ServerManager() :
	serverNum(0)
{
}

void ServerManager::Init(ServerData* data, short serverNum)
{
	serverNum = serverNum;
	for (size_t i = 0; i < serverNum; ++i)
	{
		ServerData* singleData = &data[i];
		auto serverInstancePtr = CreateServer(singleData);
		serverMap.insert(std::make_pair(singleData, serverInstancePtr));
	}
}

std::shared_ptr<Server> ServerManager::CreateServer(ServerData* data)
{
	std::shared_ptr<Server> index = 0;
	switch (data->serverType)
	{
	case TcpServerType:
		index = std::make_shared<TcpServer>(IOServerPool::GetInstance()->GetIoContext(), data);
		break;
	case HttpServerType:
		index = std::make_shared<HttpServer>(IOServerPool::GetInstance()->GetIoContext(), data);
		break;
	case WebServerType:
		break;
	default:
		break;
	}
	if (index != nullptr)
	{
		index->Start();
	}
	return index;
}

Server* ServerManager::GetServer(ServerType serverType)
{
	auto iteBegin = serverMap.begin();
	for (size_t i = 0; i < serverMap.size(); ++i)
	{
		if (iteBegin->first->serverType == serverType)
			return iteBegin->second.get();
		++iteBegin;
	}
	return nullptr;
}

ServerData* ServerManager::GetData(ServerType serverType)
{
	auto iteBegin = serverMap.begin();
	for (size_t i = 0; i < serverMap.size(); ++i)
	{
		if (iteBegin->first->serverType == serverType)
			return iteBegin->first;
		++iteBegin;
	}
	return nullptr;
}

void ServerManager::CloseServer(ServerType serverType)
{
	auto server = GetServer(serverType);
	auto data = GetData(serverType);
	server->Stop();
	//单体说明程序还在运行，需要回收
	if (data != nullptr)
		delete data;
	serverMap.erase(data);
}

void ServerManager::CloseServer()
{
	auto server = serverMap.begin();
	for (size_t i = 0; i < serverMap.size(); i++)
	{
		server->second->Stop();
		++server;
	}
	serverMap.clear();
}
