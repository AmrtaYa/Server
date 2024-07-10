#pragma once
#include "Server.h"
#include "BoostDefine.h"
#include "boost/asio.hpp"
#include "boost/beast.hpp"
#include "memory"
class HttpServer :public Server,public std::enable_shared_from_this<HttpServer>
{
public :
	HttpServer(IO_Context& io_content, ServerData* serverConfig);
	void Start() override;
	void Stop() override;
protected:
	std::shared_ptr<TcpAcceptor> acceptor;
	std::shared_ptr<Connect> CreateNewConnect() override;
	void HandleAccept(std::shared_ptr<Connect> connect, const ErrorCode& ec) override;
};

