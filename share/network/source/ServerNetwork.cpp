#include "ServerNetwork.h"

ServerNetwork::ServerNetwork(int nPort, int nPoolNum, int second_check):
net_comm(second_check)
{
	m_pTcpServer = boost::make_shared<tcp_server>(nPort, this, nPoolNum);
}


ServerNetwork::~ServerNetwork()
{

}

void ServerNetwork::Start()
{
	proc_registerpacket();
	start();
	m_pTcpServer->start();
}

void ServerNetwork::Run()
{
	proc_registerpacket();
	start();
	m_pTcpServer->run();
}


void ServerNetwork::check_session_status()
{
	send_heart_packet();
}

//该回调函数只管发送心跳包
void ServerNetwork::send_heart_packet()
{
	heart_packet packet;
	vector_session_type::iterator iter = m_tcp_sessions.begin();
	for (; iter != m_tcp_sessions.end(); ++iter)
	{
		send_packet(*iter, packet);
	}
}

