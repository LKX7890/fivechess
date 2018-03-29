#ifndef _TCP_CLIENT_H
#define _TCP_CLIENT_H

#include "dllexport.h"
#include "tcp_comm.hpp"

//tcp_client��TCP�ͻ��˵�����
//��Ҫ������1.ʹ��asio::ip::tcp::socket���ӷ�����
//          2.һ�������Ϸ����������̹߳��𣬷�����̼߳�������
class NETWORK_API tcp_client:public tcp_comm
{
public:
	tcp_client(tcp_type::endpoint ep, net_comm *net, int n = 4);
private:
	void start_connect();

	void handle_connect(const boost::system::error_code &error, tcp_session_ptr session);
private:

	tcp_type::endpoint m_endpoint;
	scheduler_type   m_schedule;   //��Ҫ�������ӷ������Ĳ�����ÿ��1����1��
	tcp_session_ptr m_session;     //��Ҫ������������ĻỰ
};



#endif