#include "tcp_server.hpp"

#include <boost/bind.hpp>
#include <boost/functional/factory.hpp>

using namespace boost;
using namespace boost::asio;


tcp_server::tcp_server(ushort_t port, 
					   net_comm *net, 
					   int n):
         tcp_comm(net,n),
		 m_acceptor(m_ios_pool.get(), ip::tcp::endpoint(ip::tcp::v4(),port))
{
	start_accept();
}

//����tcp_sessionָ�룬Ȼ������acceptor���첽�������ӣ���bind��
//�˽����¼�����ʱ�Ĵ�����
void tcp_server::start_accept()
{
	tcp_session_ptr session = 
		factory<tcp_session_ptr>()(m_ios_pool.get(), m_queue);

	session->set_callbackfunc(m_connectfunc, m_disconnectfunc);

	//step1-->��������
	
	m_acceptor.async_accept(session->socket(),                       //�����ܵ���socket������session��socket��
		                    boost::bind(&tcp_server::handle_accept, 
		                                 this,
										 placeholders::error, 
										 session));
	//boost::asio::ip::tcp::socket &sock = session->socket();
}

void tcp_server::handle_accept(const boost::system::error_code &error, tcp_session_ptr session)
{
	//����һ���µ��첽��������
	start_accept();

	//�����ӷ����˴��󣬼��Ͽ������ӣ���������Ϣ
	if (error) 
	{
		session->close();
		return;
	}

	//step2-->����tcp����
	session->start();
	//session->io_service().dispatch(boost::bind(&tcp_session::start, m_handler));
}

