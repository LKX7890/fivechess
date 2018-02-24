#ifndef _CLIENT_NETWORK_H
#define _CLIENT_NETWORK_H


#include "net_comm.hpp"
#include "tcp_client.hpp"

class NETWORK_API ClientNetwork:public net_comm
{
	typedef boost::asio::ip::tcp  tcp_type;
public:
	ClientNetwork(tcp_type::endpoint ep, int nServicePoolNum = 4, int second_check = 1);
	virtual ~ClientNetwork();
public:
	//异步
	void Start();

	//同步
	void Run();
public:
	//处理有会话端连接上
	virtual bool proc_connect(tcp_session_ptr &session) = 0;

	//处理收到网络的包
	virtual bool proc_packet(tcp_session_ptr &session, serial_packet *packet) = 0;

	///处理会话端断开连接
	virtual bool proc_disconnect(tcp_session_ptr &session) = 0;

	//注册要处理的包类型
	virtual void proc_registerpacket() = 0;

	//探测会话端状态
	virtual void check_session_status();
private:
	boost::shared_ptr<tcp_client> m_pTcpClient;
};



#endif