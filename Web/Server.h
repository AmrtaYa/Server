#pragma once
#include "BoostDefine.h"
#include "Connect.h"
#include "vector"
#include "map"
#include "boost/asio.hpp"
#include "memory"
enum ServerType
{
	TcpServerType,
	HttpServerType,
	WebServerType
};
struct ServerData
{
public:
	ServerData(const char* ip, short port, ServerType serverType);
	ServerType serverType;
	 const char* ipStr;
	 short port;
};
/// <summary>
/// 服务器管理
/// </summary>
class Server
{
public:
	Server(IO_Context& io_content,ServerData* serverConfig);
	virtual void Start() ;
	virtual void Stop() ;
	std::vector<Connect*> GetAllConnects();
	virtual void RemoveConnect(std::string id);//移除连接
	virtual void Send(SendNode* node);
protected:
	std::map<std::string,std::shared_ptr<Connect>> connects;
	IO_Context& io;//用来接受客户端的上下文
	ServerData* serverConfig;
	std::mutex mut;
	virtual void HandleAccept(std::shared_ptr<Connect> connect,const ErrorCode& ec );
	virtual std::shared_ptr<Connect> CreateNewConnect();
	virtual void AddConnect(std::string id);//添加连接



};

