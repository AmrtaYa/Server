#include "HttpServer.h"
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/random_generator.hpp>
#include "HttpConnect.h"
#include "IOServerPool.h"
#include "LogManager.h"
HttpServer::HttpServer(IO_Context& io_content, ServerData* serverConfig) :
	Server(io_content, serverConfig),
	acceptor(std::make_shared<TcpAcceptor>(io_content, EndPoint(boost::asio::ip::address::from_string(serverConfig->ipStr), serverConfig->port)))
{
}

void HttpServer::Start()
{
	std::string startStr(serverConfig->ipStr);
	startStr += ":" + (std::to_string(serverConfig->port));
	startStr += "\t HttpServer Start";
	LogManager::Log(startStr.c_str());

	auto newConnect = CreateNewConnect();
	auto p = HttpServer::shared_from_this();
	acceptor->async_accept(newConnect->GetSocket(), [p, newConnect](ErrorCode ec) {
		p->HandleAccept(newConnect, ec);
		});
}

void HttpServer::Stop()
{
}

std::shared_ptr<Connect> HttpServer::CreateNewConnect()
{
	boost::uuids::uuid id = boost::uuids::random_generator()();
	std::string idStr = boost::uuids::to_string(id);
	std::shared_ptr<HttpConnect> index = std::make_shared<HttpConnect>(IOServerPool::GetInstance()->GetIoContext(), idStr);
	return index;
}

void HttpServer::HandleAccept(std::shared_ptr<Connect> connect, const ErrorCode& ec)
{
	auto ip = connect->GetSocket().remote_endpoint().address().to_string();
	if (!ec)
	{
		LogManager::Log(ip + "连接至Http服务器");
		connect->Start();
		connects.insert(std::make_pair(connect->GetID(), connect));
	}
	else
	{
		LogManager::Log(ip + "连接Http服务器失败，报错代码为 : " + ec.message());
	}
	auto newConnect = CreateNewConnect();
	acceptor->async_accept(newConnect->GetSocket(), [this, newConnect](ErrorCode ec) {
		HandleAccept(newConnect, ec);
		});
}
