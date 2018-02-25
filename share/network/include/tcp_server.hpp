#ifndef _TCP_SERVER_H
#define _TCP_SERVER_H

#include "dllexport.h"
#include "tcp_comm.hpp"

//tcp_server��TCP������������
//��Ҫ������1.ʹ��asio::ip::tcp::acceptor�����˿�
//          2.һ���������¼������ʹ���tcp_session���в�������

class  NETWORK_API tcp_server:public tcp_comm
{
public:
	tcp_server(ushort_t port, net_comm *net, int n = 4);

private:
	//�����˿ڼ������첽��������
	void start_accept();

	//���յ��첽������
	void handle_accept(const boost::system::error_code &error, tcp_session_ptr session);

private:

	//������
	acceptor_type m_acceptor;
};



#endif