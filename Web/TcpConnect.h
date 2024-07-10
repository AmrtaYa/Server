#pragma once
#include "BoostDefine.h"
#include "Connect.h"
#include "boost/asio.hpp"
#include "MsgNode.h"

class TcpConnect : public Connect
{
public:
	TcpConnect(IO_Context& io, std::string uuid);
	void Start() override;
	void Close() override;
	 void Write(SendNode* node) override ;
private:
	std::shared_ptr<HeadMsgNode> headNode;
	std::shared_ptr<MsgNode> msgNode;
	void HandleHeadMsg(const boost::system::error_code& error, size_t  bytes_transferred, std::shared_ptr<Connect> shared_self);
	void HandleMsg(const boost::system::error_code& error, size_t  bytes_transferred, std::shared_ptr<Connect> shared_self);

	void HandleWrite(const boost::system::error_code& error, SendNode* node);

};

