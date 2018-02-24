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
	//�첽
	void Start();

	//ͬ��
	void Run();
public:
	//�����лỰ��������
	virtual bool proc_connect(tcp_session_ptr &session) = 0;

	//�����յ�����İ�
	virtual bool proc_packet(tcp_session_ptr &session, serial_packet *packet) = 0;

	///����Ự�˶Ͽ�����
	virtual bool proc_disconnect(tcp_session_ptr &session) = 0;

	//ע��Ҫ����İ�����
	virtual void proc_registerpacket() = 0;

	//̽��Ự��״̬
	virtual void check_session_status();
private:
	boost::shared_ptr<tcp_client> m_pTcpClient;
};



#endif