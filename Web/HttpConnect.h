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
	/// 检查过时时间
	/// </summary>
	void CheckDeadLine();
	void AcceptRequest();
	void HandleRequest();


	void PorcessGetRequest();
	void PorcessPostRequest();

	boost::beast::flat_buffer buffer{ HTTPBUFFER };
	HttpRequest request;//请求
	HttpRespond respond;//回应
	short deadLineTime;//超时时间
	Timer deadLine;

	
};

