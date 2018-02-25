#ifndef _TCP_COMM_H
#define _TCP_COMM_H

#include "dllexport.h"
#include "thread_header.hpp"
#include "msg_def.hpp"
#include "tcp_session.hpp"
#include "io_service_pool.hpp"
#include "net_comm.hpp"
#include "scheduler.hpp"

class NETWORK_API tcp_comm
{
public:
	typedef io_service_pool::ios_type ios_type;
	typedef boost::asio::ip::tcp::acceptor acceptor_type;
	typedef boost::asio::ip::tcp  tcp_type;
	typedef tcp_session::queue_type queue_type;
	typedef boost::function<bool(tcp_session_ptr &)>  function_type;
	typedef scheduler scheduler_type;
public:
	tcp_comm(net_comm *net,
			 int n = 4):
	    m_ios_pool(*factory<io_service_pool*>()(n)),
	    m_queue(net->recv_queue()),
		m_connectfunc(boost::bind(&net_comm::session_connected, net, _1)),
		m_disconnectfunc(boost::bind(&net_comm::session_disconnected, net, _1))
	{
	}
public:
	void start()
	{
		m_ios_pool.start();
	}
	void run()
	{
		m_ios_pool.run();
	}
protected:
	io_service_pool &m_ios_pool; //并发线程池
	function_type m_connectfunc;
	function_type m_disconnectfunc;
	queue_type &m_queue;         //工作队列，即消息队列
};




#endif