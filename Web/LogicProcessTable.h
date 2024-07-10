#pragma once
#include <memory>
#include "json/value.h"
#include "string"
class Connect;
class LogicNode;
class HttpConnect;
static class LogicProcessTable
{
public:
	static void  TCP_ProcessEchoTest_1001( std::shared_ptr<LogicNode> logicValue);
};
static class HttpLogicPorcessTable
{
public:
	static void  Http_ProcessHtml(std::shared_ptr<Connect> http, std::string data);
	static void  Http_ProcessJs(std::shared_ptr<Connect> http, std::string data);
	static void  Http_ProcessJPGImg(std::shared_ptr<Connect> http, std::string data);
	static void  Http_ProcessPNGImg(std::shared_ptr<Connect> http, std::string data);
	static void  Http_ProcessCSS(std::shared_ptr<Connect> http, std::string data);


	static void  Http_ProcessDBConfirm(std::shared_ptr<Connect> http, std::string data);
};

