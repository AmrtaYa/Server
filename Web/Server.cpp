#include "Server.h"
#include "iostream"
Server::Server(IO_Context& io_content, ServerData* serverConfig):
	io(io_content),serverConfig(serverConfig)
{
}

void Server::Start()
{

}

void Server::Stop()
{
}
/// <summary>
/// ��ȡ���з�����������
/// </summary>
std::vector<Connect*> Server::GetAllConnects()
{
	std::vector<Connect*> indexVec;
	for (auto i = connects.begin(); i != connects.end(); i++)
	{
		indexVec.push_back(i->second.get());
	}
	return indexVec;
}

void Server::HandleAccept(std::shared_ptr<Connect> connect, const ErrorCode& ec)
{
}

std::shared_ptr<Connect> Server::CreateNewConnect()
{
	return std::shared_ptr<Connect>();
}

void Server::AddConnect(std::string id)
{
}

void Server::RemoveConnect(std::string id)
{
}
/// <summary>
/// ���䱾���ֽڳ��ȼ��ɣ�����Ҫ�����ֽ�
/// </summary>
/// <param name="msg"></param>
/// <param name="len"></param>
void Server::Send(SendNode* node)
{
}

ServerData::ServerData(const char* ip, short port, ServerType serverType):
	ipStr(ip),port(port),serverType(serverType)
{
}
