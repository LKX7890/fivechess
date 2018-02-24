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

//创建tcp_session指针，然后启动acceptor的异步接收连接，用bind绑定
//了接受事件发生时的处理函数
void tcp_server::start_accept()
{
	tcp_session_ptr session = 
		factory<tcp_session_ptr>()(m_ios_pool.get(), m_queue);

	session->set_callbackfunc(m_connectfunc, m_disconnectfunc);

	//step1-->接受连接
	
	m_acceptor.async_accept(session->socket(),                       //将接受到的socket保存在session的socket中
		                    boost::bind(&tcp_server::handle_accept, 
		                                 this,
										 placeholders::error, 
										 session));
	//boost::asio::ip::tcp::socket &sock = session->socket();
}

void tcp_server::handle_accept(const boost::system::error_code &error, tcp_session_ptr session)
{
	//启动一个新的异步接收连接
	start_accept();

	//该连接发生了错误，即断开该连接，并返回信息
	if (error) 
	{
		session->close();
		return;
	}

	//step2-->启动tcp连接
	session->start();
	//session->io_service().dispatch(boost::bind(&tcp_session::start, m_handler));
}

