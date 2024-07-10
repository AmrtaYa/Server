#pragma once
#include "Connect.h"
#include "boost/beast.hpp"
#include "BoostDefine.h"
#include "MsgDefine.h"
class HttpConnect:public Connect
{
public:
	HttpConnect(IO_Context& io, std::string uuid);
	void Start() override;
	void Close() override;
	HttpRespond& GetRespond();
	void WriteResponse();
private:
	/// <summary>
	/// ����ʱʱ��
	/// </summary>
	void CheckDeadLine();
	void AcceptRequest();
	void HandleRequest();


	void PorcessGetRequest();
	void PorcessPostRequest();

	boost::beast::flat_buffer buffer{ HTTPBUFFER };
	HttpRequest request;//����
	HttpRespond respond;//��Ӧ
	short deadLineTime;//��ʱʱ��
	Timer deadLine;

	
};

