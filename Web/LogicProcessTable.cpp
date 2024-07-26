#include "LogicProcessTable.h"
#include "ServerManager.h"
#include "iostream"
#include "LogicSystem.h"
#include "HttpConnect.h"
#include "FileHelper.h"
#include "DbManager.h"
void LogicProcessTable::TCP_ProcessEchoTest_1001(std::shared_ptr<LogicNode> nodeValue)
{
	auto msg = nodeValue->GetWholeJsonMsg();
	std::cout << "收到了消息 : " << msg << std::endl;

	ServerManager::GetInstance()->GetServer(TcpServerType)->Send(new SendNode(nodeValue->GetWholeJsonLen(), msg.c_str()));
}

void HttpLogicPorcessTable::Http_ProcessHtml(std::shared_ptr<Connect> http, std::string data)
{
	auto connect = static_cast<HttpConnect*>(http.get());
	HttpRespond& respond = connect->GetRespond();
	respond.set(boost::beast::http::field::content_type, "text/html");
	boost::beast::ostream(respond.body()) << FileHelper::ReadText(data + ".html");

}

void HttpLogicPorcessTable::Http_ProcessJs(std::shared_ptr<Connect> http, std::string data)
{
	auto connect = static_cast<HttpConnect*>(http.get());
	HttpRespond& respond = connect->GetRespond();
	respond.set(boost::beast::http::field::content_type, "text/javascript");
	boost::beast::ostream(respond.body()) << FileHelper::ReadText(data + ".js");
}

void HttpLogicPorcessTable::Http_ProcessJPGImg(std::shared_ptr<Connect> http, std::string data)
{
	auto connect = static_cast<HttpConnect*>(http.get());
	HttpRespond& respond = connect->GetRespond();
	respond.set(boost::beast::http::field::content_type, "image/jpg");
	respond.set(boost::beast::http::field::access_control_allow_origin, "*");
	boost::beast::ostream(respond.body()) << FileHelper::ReadBytes(data + ".jpg");
	respond.prepare_payload();
}

void HttpLogicPorcessTable::Http_ProcessPNGImg(std::shared_ptr<Connect> http, std::string data)
{
	auto connect = static_cast<HttpConnect*>(http.get());
	HttpRespond& respond = connect->GetRespond();
	respond.set(boost::beast::http::field::content_type, "image/jpg");
	respond.set(boost::beast::http::field::access_control_allow_origin, "*");
	boost::beast::ostream(respond.body()) << FileHelper::ReadBytes(data + ".png");
	respond.prepare_payload();
}

void HttpLogicPorcessTable::Http_ProcessCSS(std::shared_ptr<Connect> http, std::string data)
{
	auto connect = static_cast<HttpConnect*>(http.get());
	HttpRespond& respond = connect->GetRespond();
	respond.set(boost::beast::http::field::content_type, "text/css");
	respond.set(boost::beast::http::field::access_control_allow_origin, "*");
	boost::beast::ostream(respond.body()) << FileHelper::ReadText(data + ".css");
}
/// <summary>
/// 确认数据库等问题
/// </summary>
/// <param name="http"></param>
/// <param name="data"></param>
void HttpLogicPorcessTable::Http_ProcessDBConfirm(std::shared_ptr<Connect> http, std::string data)
{
	auto connect = static_cast<HttpConnect*>(http.get());
	HttpRespond& respond = connect->GetRespond();
	respond.set(boost::beast::http::field::content_type, "text/plain");
	respond.set(boost::beast::http::field::access_control_allow_origin, "*");

	size_t pos = data.find_last_of("?");
	data = data.substr(pos + 1, data.length());
	size_t andPos = data.find_last_of("&");
	std::string userStr = data.substr(0, andPos);
	std::string pwdStr = data.substr(andPos + 1, data.length());

	auto userEqPos = userStr.find_first_of('=') + 1;
	auto pwdEqPos = pwdStr.find_first_of('=') + 1;
	userStr = userStr.substr(userEqPos, userStr.length());
	pwdStr = pwdStr.substr(pwdEqPos, pwdStr.length());

	//这边可能需要防注入一下
	std::stringstream sstream;
	sstream << "Account = '" << userStr << "' AND " << "Pwd = '" << pwdStr << "'";
	auto entity = DB_FIND(sstream.str(), "*", Entity::UserInfo);
	if (!entity.empty())
	{
		LogManager::Log("账号验证成功，账号为:" + userStr);
		boost::beast::ostream(respond.body()) << TRUE;
	}
	else
	{
		LogManager::Log("账号验证失败，账号为:" + userStr);
		boost::beast::ostream(respond.body()) << FALSE;
	}
	
}
