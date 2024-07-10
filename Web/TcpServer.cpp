#include "TcpServer.h"
#include "LogManager.h"
#include "boost/uuid/random_generator.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "IOServerPool.h"
#include "TcpConnect.h"
#include "json/reader.h"
#include "json/value.h"
TcpServer::TcpServer(IO_Context& io_content, ServerData* serverConfig)
	: Server(io_content, serverConfig), acceptor
	(std::make_shared<TcpAcceptor>(io, EndPoint(boost::asio::ip::address::from_string(serverConfig->ipStr), serverConfig->port)))

{
}

void TcpServer::Start()
{
	std::string startStr ( serverConfig->ipStr);
	startStr +=":"+(std::to_string(serverConfig->port));
	startStr += "\t TcpServer Start";
	LogManager::Log(startStr.c_str());
	auto newConnect = CreateNewConnect();
	auto p = TcpServer::shared_from_this();
	acceptor->async_accept(newConnect->GetSocket(), [p, newConnect](ErrorCode ec) {
		p->HandleAccept(newConnect, ec);
		});
}

void TcpServer::Stop()
{
	LogManager::Log("TcpServer Stop");
}

void TcpServer::RemoveConnect(std::string id)
{
	connects[id]->Close();
	connects.erase(id);
}
/// <summary>
/// 会将其转化为网络字节
/// </summary>
/// <param name="msg"></param>
/// <param name="len"></param>
void TcpServer::Send(SendNode* node)
{
	bool pend = false;//是否堵塞，堵塞就只加入队列，但不处理，等上个队列处理完再处理
	std::lock_guard<std::mutex> lock(mut);
	if (sendQue.size() > 0)
	{
		pend = true;
	}
	sendQue.emplace(std::make_shared<SendNode>(*node));

	if (pend)return;
	HandleWriteQueue();
}

void TcpServer::HandleAccept(std::shared_ptr<Connect> connect, const ErrorCode& ec)
{
	auto ip = connect->GetSocket().local_endpoint().address().to_string();
	if (!ec)
	{
		LogManager::Log(ip + "连接至TCP服务器");
		connect->Start();
		connects.insert(std::make_pair(connect->GetID(), connect));
	}
	else
	{
		LogManager::Log(ip + "连接TCP服务器失败，报错代码为 : " + ec.message());
	}
	auto newConnect = CreateNewConnect();
	acceptor->async_accept(newConnect->GetSocket(), [this, newConnect](ErrorCode ec) {
		HandleAccept(newConnect, ec);
		});
}

void TcpServer::HandleWriteQueue()
{
	while (sendQue.size()!=0)
	{
		auto sendMsg = sendQue.front();
		for (auto i = connects.begin(); i != connects.end(); ++i)
		{
			auto con = i->second;
			con->Write(sendMsg.get());
		}
		sendQue.pop();
	}
}

std::shared_ptr<Connect> TcpServer::CreateNewConnect()
{
	boost::uuids::uuid id = boost::uuids::random_generator()();
	std::string idStr = boost::uuids::to_string(id);
	std::shared_ptr<TcpConnect> index = std::make_shared<TcpConnect>(IOServerPool::GetInstance()->GetIoContext(), idStr);
	return index;
}

