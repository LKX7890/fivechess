#ifndef _TCP_SERVER_H
#define _TCP_SERVER_H

#include "dllexport.h"
#include "tcp_comm.hpp"

//tcp_server是TCP服务器的引擎
//主要工作：1.使用asio::ip::tcp::acceptor监听端口
//          2.一旦有连接事件发生就创建tcp_session进行并发处理

class  NETWORK_API tcp_server:public tcp_comm
{
public:
	tcp_server(ushort_t port, net_comm *net, int n = 4);

private:
	//启动端口监听，异步接收连接
	void start_accept();

	//接收的异步处理函数
	void handle_accept(const boost::system::error_code &error, tcp_session_ptr session);

private:

	//接收器
	acceptor_type m_acceptor;
};



#endif