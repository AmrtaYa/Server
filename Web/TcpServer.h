#pragma once
#include "Server.h"
#include "BoostDefine.h"
#include "MsgNode.h"
#include "queue"
class TcpServer : public Server, public std::enable_shared_from_this<TcpServer>
{
public:
	TcpServer(IO_Context& io_content, ServerData* serverConfig);
	void Start() override;
	void Stop() override;
	 void RemoveConnect(std::string id) override;//ÒÆ³ýÁ¬½Ó
	  void Send(SendNode* node) override;
protected:
	std::shared_ptr<TcpAcceptor> acceptor;
	 void HandleAccept(std::shared_ptr<Connect> connect, const ErrorCode& ec) override;
	 void HandleWriteQueue();
	  std::shared_ptr<Connect> CreateNewConnect() override;
	  std::queue< std::shared_ptr<SendNode>> sendQue;
private:

};

