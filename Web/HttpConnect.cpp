#include "HttpConnect.h"
#include "LogManager.h"
#include "chrono"
#include "ServerManager.h"
#include "LogicSystem.h"
HttpConnect::HttpConnect(IO_Context& io, std::string uuid)
	: Connect(io, uuid), deadLineTime(60), deadLine(socket->get_executor(), std::chrono::seconds(deadLineTime))
{
}

void HttpConnect::Start()
{
	AcceptRequest();
	CheckDeadLine();
}

void HttpConnect::Close()
{
	ErrorCode ec;
	socket->shutdown(Tcp::socket::shutdown_send, ec);
	deadLine.cancel();
	ServerManager::GetInstance()-> GetServer(HttpServerType)->RemoveConnect(this->GetID()) ;
}

HttpRespond& HttpConnect::GetRespond()
{
	return respond;
}

void HttpConnect::CheckDeadLine()
{
	auto self = shared_from_this();
	deadLine.async_wait([self](ErrorCode ec) {
		if (!ec)
			self->Close();
		});
}

void HttpConnect::AcceptRequest()
{
	auto self = shared_from_this().get();
	auto httpSelf = static_cast<HttpConnect*>(self);
	boost::beast::http::async_read(*socket.get(), buffer, request, [httpSelf](ErrorCode ec, std::size_t bytes_transferred)
		{
			boost::ignore_unused(bytes_transferred);
			if (!ec)
				httpSelf->HandleRequest();
			else
				LogManager::Log(ec.what());
		});
}

void HttpConnect::HandleRequest()
{
	respond.version(request.version());
	respond.keep_alive(false);
	switch (request.method())
	{
	case boost::beast::http::verb::get:
		respond.result(boost::beast::http::status::ok);
		respond.set(boost::beast::http::field::server,"Beast");
		PorcessGetRequest();
		break;
	case boost::beast::http::verb::post:
		respond.result(boost::beast::http::status::ok);
		respond.set(boost::beast::http::field::server, "Beast");
		PorcessPostRequest();
		break;
	default:
		break;
	}
	WriteResponse();
}

void HttpConnect::WriteResponse()
{
	auto self = shared_from_this().get();

	respond.content_length(respond.body().size());
	boost::beast::http::async_write(GetSocket(), respond, [self](ErrorCode ec, std::size_t) {
						self->Close();
		});

}

void HttpConnect::PorcessGetRequest()
{
	std::string target = request.target();
	auto self =shared_from_this();
	 LogicSystem::GetInstance()->ProcessHttpCallBack(self, target.c_str());
}

void HttpConnect::PorcessPostRequest()
{
	std::string target = request.target();
	auto self = shared_from_this();
	LogicSystem::GetInstance()->ProcessHttpCallBack(self, target.c_str());
}
