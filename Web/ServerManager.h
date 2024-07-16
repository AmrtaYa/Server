#pragma once
#include "Singleton.h"
#include "Server.h"
#include "vector"
#include "map"
class ServerManager : public Singleton<ServerManager>
{
	friend class Singleton<ServerManager>;
private:
	ServerManager();
	short serverNum;
	std::map <ServerData*, std::shared_ptr<Server>> serverMap;
	std::shared_ptr<Server> CreateServer(ServerData* data);
public:
	void Init(ServerData* data, short serverNum);
	Server* GetServer(ServerType serverType);
	ServerData* GetData(ServerType serverType);
	/// <summary>
	/// �ر�ָ��������
	/// </summary>
	/// <param name="serverType"></param>
	void CloseServer(ServerType serverType);
	/// <summary>
	/// �ر����з�����
	/// </summary>
	void CloseServer();
};

