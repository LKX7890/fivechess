#include "net_comm.hpp"
#include <boost/functional/factory.hpp>
#include "tcp_session.hpp"

#pragma warning(disable:4355)
#pragma warning(disable:4996)

net_comm::net_comm(int sec_check):
m_req_worker(m_recvq), m_resp_worker(m_sendq)
{
	m_req_worker.set_workerFunc(boost::bind(&net_comm::process_msg, this, _1));
	m_resp_worker.set_workerFunc(boost::bind(&net_comm::send_msg, this, _1));


	//ÿ��sec_check����Ự�˷���һ�μ���
	m_schedule.add(boost::bind(&net_comm::check_session_status, this), sec_check);

	//��ʼ
	m_schedule.start();
}


//���������߳�
void net_comm::start()
{
	m_req_worker.start();
	m_resp_worker.start();
}


net_comm::req_queue_type &net_comm::recv_queue()
{
	return m_recvq;
}


//�������������ǹ����õĻص�����
bool net_comm::process_msg(tcp_request_ptr &req)
{
	msg_head *head = req->get_head();
	serial_packet *p = m_packetfactory.generate_packet(static_cast<packet_type>(head->type), req->msg_data(), head->size);
	if (p)
	{
		return proc_packet(req->get_session(), p);
	}
	return true;
	
}


bool net_comm::send_msg(tcp_response_ptr &resp)
{
	resp->set_msg_crc();
	resp->get_session()->write(resp);
	return true;
}

bool net_comm::session_connected(tcp_session_ptr &session)
{
	proc_connect(session);
	return add_session(session);
}

bool net_comm::session_disconnected(tcp_session_ptr &session)
{
	proc_disconnect(session);
	return remove_session(session);
}


bool net_comm::send_packet(tcp_session_ptr &sp, serial_packet &packet)
{
	//ÿһ�η���֮ǰ�ȼ������ӵ�״̬
	if (!sp->socket().is_open() || !sp->isconnected())
	{
		sp->close();
		return false;
	}
	tcp_response_ptr resp = boost::factory<tcp_response_ptr>()(sp);
	generate_session_data(resp, packet);
	m_sendq.push(resp);
	return true;
}


void net_comm::generate_session_data(tcp_response_ptr &resp, serial_packet &packet)
{
	
	//1.���������,��packet�е��������л���buf
	char buf[SEND_BUFFER];
	memset(buf, 0, SEND_BUFFER);

	std::ostrstream sa(buf, SEND_BUFFER);

	text_oarchive oa(sa);
	packet.serial(oa);

	
	//2.������Ϣͷ
	msg_head head;
	head.type = packet.get_packet_type();

	head.size = sa.pcount();

	head.chksum = std::for_each(buf, buf + sa.pcount(), crc_32_type())();

	//3.��ֵ
	resp->get_head()->chksum = head.chksum;
	resp->get_head()->size   = head.size;
	resp->get_head()->type   = head.type;

	std::copy(buf, buf + sa.pcount(), resp->msg_data().begin());
}


bool net_comm::add_session(const tcp_session_ptr &session)
{
	//����ûỰ��û�����б��вż���
	if (!m_tcp_sessions.find(session))
	{
		m_tcp_sessions.push_back(session);
		return true;
	}
	return false;
}

bool net_comm::remove_session(const tcp_session_ptr &session)
{
	return m_tcp_sessions.remove(session);
}

void net_comm::check_session_status()
{

}
