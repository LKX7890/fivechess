#ifndef _NET_COMM_H
#define _NET_COMM_H

#include "dllexport.h"
#include "tcp_message.hpp"
#include "job_queue.hpp"
#include "worker.h"
#include "safe_map.hpp"
#include "safe_vector.h"
#include "serial_packet.h"
#include "scheduler.hpp"
#include "packet_factory.h"
#include "dllexport.h"

//������Ҫ�Ǵ����������ͻ��˹�ͬ��Ҫ�����һЩ�����ӿ�
//�����еĲ������Է�װ�ڸ�����
//����Ӧ��ֻ����ʵ�ʵ��������ӣ���������ӵĺ���Ӧ������������
class NETWORK_API net_comm
{
public:
	friend class tcp_comm;
	net_comm(int sec_check = 1);
public:
	typedef job_queue<tcp_request_ptr> req_queue_type;     //������Ϣ����
	typedef worker<req_queue_type> req_worker_type;        //�������յ�����Ϣ
	typedef job_queue<tcp_response_ptr> resp_queue_type;   //������Ϣ����
	typedef worker<resp_queue_type>   resp_worker_type;    //��������͵���Ϣ
	typedef scheduler schedule_type;
	typedef safe_vector<std::string> vector_type;
	
	typedef safe_vector<tcp_session_ptr> vector_session_type; //�������ӻỰ��
	//////////////////////////////////////////////////////////////////////////
	//���½ӿ���Ҫ�ǹ��ⲿ���ã��ṩ�����缰���ݵĿ���
public:
	void start();                  //���������߳�
	req_queue_type &recv_queue();  //������Ϣ���еķ��ʺ���
protected:
	
	//�����лỰ��������
	virtual bool proc_connect(tcp_session_ptr &session) = 0;

	//�����յ�����İ�
	virtual bool proc_packet(tcp_session_ptr &session, serial_packet *packet) = 0;

	//����Ự�˶Ͽ�
	virtual bool proc_disconnect(tcp_session_ptr &session) = 0;
	
	//ע��Ҫ����İ�����
	virtual void proc_registerpacket() = 0;

	//̽��Ự��״̬
	virtual void check_session_status();

protected:

	bool session_connected(tcp_session_ptr &session);
	bool session_disconnected(tcp_session_ptr &session);

	//��ָ���ĻỰ�˷�������
	bool send_packet(tcp_session_ptr &sp, serial_packet &packet);

	//////////////////////////////////////////////////////////////////////////
	//
	bool add_session(const tcp_session_ptr &session);
	bool remove_session(const tcp_session_ptr &session);

	template<class Packet>
	void register_packet(packet_type type)
	{
		m_packetfactory.register_packet_type<Packet>(type);
	}

private:
	//��Ϣ����ص�����
	bool process_msg(tcp_request_ptr &req);
	bool send_msg(tcp_response_ptr &resp);
	//�������л��İ������Ự����
	void generate_session_data(tcp_response_ptr &resp, serial_packet &packet);

protected:
	req_queue_type   m_recvq;           //������Ϣ�Ķ���
	resp_queue_type  m_sendq;           //�����͵���Ϣ����
	req_worker_type  m_req_worker;      //�����ѽ�����Ϣ�Ĺ����߳�
	resp_worker_type m_resp_worker;     //�����������Ϣ�Ĺ����߳�      
	packet_factory   m_packetfactory;   //�����ɹ���
	schedule_type    m_schedule;        //�����������״̬
	vector_session_type m_tcp_sessions; //���б������ظ���Ԫ��
};



#endif