#include "ClientNetwork.h"

ClientNetwork::ClientNetwork(tcp_type::endpoint ep, int nServicePoolNum, int second_check):
net_comm(second_check)
{
	m_pTcpClient = boost::make_shared<tcp_client>(ep, this, nServicePoolNum);
}


ClientNetwork::~ClientNetwork()
{

}

void ClientNetwork::Start()
{
	proc_registerpacket();
	start();
	m_pTcpClient->start();
}


void ClientNetwork::Run()
{
	proc_registerpacket();
	start();
	m_pTcpClient->run();
}





void ClientNetwork::check_session_status()
{

}