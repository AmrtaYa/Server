#include "TcpConnect.h"
#include "MsgDefine.h"
#include "ServerManager.h"
#include "LogicSystem.h"
TcpConnect::TcpConnect(IO_Context& io, std::string uuid) :
	Connect(io, uuid),
	msgNode(std::make_shared<MsgNode>()),
	headNode(std::make_shared<HeadMsgNode>())
{
}

void TcpConnect::Start()
{
	boost::asio::async_read(GetSocket(), boost::asio::buffer(headNode->GetData(), MSGHEAD), [this](auto err, auto byte_transferred)
		{
			HandleHeadMsg(err, byte_transferred, shared_from_this());
		});
}

void TcpConnect::Close()
{

}

void TcpConnect::Write(SendNode* node)
{
	//在Server中的Send已经进行网络字节转换
	//先发长度
	boost::asio::async_write(GetSocket(), boost::asio::buffer(node->GetData(), node->GetLen()), [this, node](ErrorCode err,size_t size) {
					HandleWrite(err, node);
		});
}
void TcpConnect::HandleWrite(const boost::system::error_code& error, SendNode* node)
{

}
void TcpConnect::HandleHeadMsg(const boost::system::error_code& error, size_t bytes_transferred, std::shared_ptr<Connect> shared_self)
{
	if (error)
	{
		ServerManager::GetInstance()->GetServer(TcpServerType)->RemoveConnect(this->GetID());
		return;
	}
	short dataLength = headNode->GetMsgLen();

	if (dataLength > MAXBUFFER)
	{
		ServerManager::GetInstance()->GetServer(TcpServerType)->RemoveConnect(this->GetID());
		return;
	}
	boost::asio::async_read(GetSocket(), boost::asio::buffer(msgNode->GetData(), dataLength), [this](auto err, auto byte_transferred)
		{
			HandleMsg(err, byte_transferred, shared_from_this());
		});
}

void TcpConnect::HandleMsg(const boost::system::error_code& error, size_t bytes_transferred, std::shared_ptr<Connect> shared_self)
{
	if (error)
	{
		ServerManager::GetInstance()->GetServer(TcpServerType)->RemoveConnect(this->GetID());
		return;
	}
	short msgLen = headNode->GetMsgLen();
	char* jsonMsg = msgNode->GetMsg(msgLen);
	//处理逻辑----------------------------------------------------------------------------------------------------------
	LogicSystem::GetInstance()->PostLogic(shared_self,jsonMsg,msgLen);
	//处理逻辑----------------------------------------------------------------------------------------------------------
	headNode->Clear();
	msgNode->Clear();
	boost::asio::async_read(GetSocket(), boost::asio::buffer(headNode->GetData(), MSGHEAD), [this](auto err, auto byte_transferred)
		{
			HandleHeadMsg(err, byte_transferred, shared_from_this());
		});
}


