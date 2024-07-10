#include "Connect.h"
#include "ServerManager.h"

Connect::Connect(IO_Context& io, std::string uuid):
	io(io),uuid(uuid),socket(std::make_shared<Tcp::socket>(io))
{
}

void Connect::Start()
{
}

void Connect::Close()
{
}

std::string Connect::GetID()
{
	return uuid;
}

Tcp::socket& Connect::GetSocket()
{
	return *socket;
}

void Connect::Write(SendNode* node)
{
}
