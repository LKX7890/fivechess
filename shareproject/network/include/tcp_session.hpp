#ifndef _TCP_SESSION_H
#define _TCP_SESSION_H

#include "dllexport.h"

#include "tcp_message.hpp"
#include "job_queue.hpp"
#include <boost/smart_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/pool/object_pool.hpp>  //对象池
#include "thread_header.hpp"
#include "safe_atom.hpp"
class net_comm;

//using namespace boost;
//该类负责处理网络通信，负责处理网络通信，实现了数据收发的核心功能
class NETWORK_API tcp_session:
	public boost::enable_shared_from_this<tcp_session>
{
public:
	typedef boost::asio::ip::tcp::socket socket_type;
	typedef boost::asio::io_service ios_type;
	
	typedef ios_type::strand strand_type;
	typedef job_queue<tcp_request_ptr> queue_type;            //消息队列
	typedef boost::object_pool<tcp_message> object_pool_type; //内存池

	typedef boost::function<bool(tcp_session_ptr &)>  function_type;
public:
	tcp_session(ios_type &ios, queue_type &q);

	void set_callbackfunc(function_type &connfunc, function_type &disconnfunc);

	socket_type &socket();
	ios_type &io_service();


	void start();
	void close();

	void write(tcp_response_ptr resp);
	bool isconnected();
	void setconnected(bool bFlag);
private:
	void read_data();
	tcp_request_ptr create_request(); //创建消息对象
	void read(tcp_request_ptr req);   //接收消息

	//消息头读处理函数
	void handle_read_head(const boost::system::error_code &error,
		size_t bytes_transferred, tcp_request_ptr req);

	//消息体读处理函数
	void handle_read_msg(const boost::system::error_code &error,
		size_t byte_transferred, tcp_request_ptr req);

	//消息头写处理函数
	void handle_write_head(const boost::system::error_code &error,
		size_t bytes_transferred, tcp_response_ptr resp);

	//消息体写处理函数
	void handle_write_msg(const boost::system::error_code &error,
		size_t byte_transferred, tcp_response_ptr resp);

private:
	socket_type m_socket;

	strand_type m_strand;               //asio库提供的线程概念，可以包装异步操作在多线程环境里安全地执行
	queue_type& m_queue;                //接收tcp_message的消息队列，队列里保存消息的指针而不是拷贝
	static object_pool_type m_msg_pool; //内存池
	
	safe_atom<bool> m_bconnected;


	function_type m_connect_func;
	function_type m_disconnect_func;

};

typedef boost::shared_ptr<tcp_session> tcp_session_ptr; //共享指针定义

#endif