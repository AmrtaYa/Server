#pragma once
#include "boost/asio.hpp"
#include "boost/beast.hpp"
typedef boost::asio::ip::tcp Tcp;
typedef boost::asio::io_context IO_Context;
typedef boost::asio::io_context::work IO_Work;
typedef boost::asio::ip::tcp::acceptor TcpAcceptor;
typedef boost::asio::ip::tcp::endpoint EndPoint;
typedef boost::system::error_code ErrorCode;
//HTTP
typedef boost::beast::flat_buffer FlatHttpBuffer;
typedef boost::beast::http::request<boost::beast::http::dynamic_body> HttpRequest;
typedef boost::beast::http::response<boost::beast::http::dynamic_body> HttpRespond;
typedef boost::beast::error_code BeastErrCode;

typedef boost::asio::steady_timer Timer;
