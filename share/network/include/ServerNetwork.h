#ifndef _SERVER_NETWORK_H
#define _SERVER_NETWORK_H

#include "dllexport.h"
#include "net_comm.hpp"
#include "tcp_server.hpp"

//服务器提供给外部的接口
class NETWORK_API ServerNetwork:public net_comm
{
public:
	ServerNetwork(int nPort, int nPoolNum = 4, int second_check = 3);
	virtual ~ServerNetwork();

public:
	//异步
	void Start();

	//同步
	void Run();
protected:
	//处理有会话端连接上
	virtual bool proc_connect(tcp_session_ptr &session) = 0;

	//处理网络包
	virtual bool proc_packet(tcp_session_ptr &session, serial_packet *packet) = 0;

	//处理会话端断开连接
	virtual bool proc_disconnect(tcp_session_ptr &session) = 0;

	//注册要处理的包类型
	virtual void proc_registerpacket() = 0;

	//检测网络连接状态
	virtual void check_session_status();

	
private:
	//发送心跳包
	void send_heart_packet();

	//检测客户端连接状态
	bool check_status();
private:
	boost::shared_ptr<tcp_server> m_pTcpServer;
};


#endif