#ifndef _SERVER_NETWORK_H
#define _SERVER_NETWORK_H

#include "dllexport.h"
#include "net_comm.hpp"
#include "tcp_server.hpp"

//�������ṩ���ⲿ�Ľӿ�
class NETWORK_API ServerNetwork:public net_comm
{
public:
	ServerNetwork(int nPort, int nPoolNum = 4, int second_check = 3);
	virtual ~ServerNetwork();

public:
	//�첽
	void Start();

	//ͬ��
	void Run();
protected:
	//�����лỰ��������
	virtual bool proc_connect(tcp_session_ptr &session) = 0;

	//���������
	virtual bool proc_packet(tcp_session_ptr &session, serial_packet *packet) = 0;

	//����Ự�˶Ͽ�����
	virtual bool proc_disconnect(tcp_session_ptr &session) = 0;

	//ע��Ҫ����İ�����
	virtual void proc_registerpacket() = 0;

	//�����������״̬
	virtual void check_session_status();

	
private:
	//����������
	void send_heart_packet();

	//���ͻ�������״̬
	bool check_status();
private:
	boost::shared_ptr<tcp_server> m_pTcpServer;
};


#endif