#ifndef _TCP_CLIENT_H
#define _TCP_CLIENT_H

#include "dllexport.h"
#include "tcp_comm.hpp"

//tcp_client是TCP客户端的引擎
//主要工作：1.使用asio::ip::tcp::socket连接服务器
//          2.一旦连接上服务器，该线程挂起，否则该线程继续连接
class NETWORK_API tcp_client:public tcp_comm
{
public:
	tcp_client(tcp_type::endpoint ep, net_comm *net, int n = 4);
private:
	void start_connect();

	void handle_connect(const boost::system::error_code &error, tcp_session_ptr session);
private:

	tcp_type::endpoint m_endpoint;
	scheduler_type   m_schedule;   //主要处理连接服务器的操作，每隔1秒连1次
	tcp_session_ptr m_session;     //主要处理与服务器的会话
};



#endif