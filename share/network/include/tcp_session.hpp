#ifndef _TCP_SESSION_H
#define _TCP_SESSION_H

#include "dllexport.h"

#include "tcp_message.hpp"
#include "job_queue.hpp"
#include <boost/smart_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/pool/object_pool.hpp>  //�����
#include "thread_header.hpp"
#include "safe_atom.hpp"
class net_comm;

using namespace boost;
//���ฺ��������ͨ�ţ�����������ͨ�ţ�ʵ���������շ��ĺ��Ĺ���
class NETWORK_API tcp_session:
	public boost::enable_shared_from_this<tcp_session>
{
public:
	typedef boost::asio::ip::tcp::socket socket_type;
	typedef boost::asio::io_service ios_type;
	
	typedef ios_type::strand strand_type;
	typedef job_queue<tcp_request_ptr> queue_type;            //��Ϣ����
	typedef boost::object_pool<tcp_message> object_pool_type; //�ڴ��

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
	tcp_request_ptr create_request(); //������Ϣ����
	void read(tcp_request_ptr req);   //������Ϣ

	//��Ϣͷ��������
	void handle_read_head(const boost::system::error_code &error,
		size_t bytes_transferred, tcp_request_ptr req);

	//��Ϣ���������
	void handle_read_msg(const boost::system::error_code &error,
		size_t byte_transferred, tcp_request_ptr req);

	//��Ϣͷд������
	void handle_write_head(const boost::system::error_code &error,
		size_t bytes_transferred, tcp_response_ptr resp);

	//��Ϣ��д������
	void handle_write_msg(const boost::system::error_code &error,
		size_t byte_transferred, tcp_response_ptr resp);

private:
	socket_type m_socket;

	strand_type m_strand;               //asio���ṩ���̸߳�����԰�װ�첽�����ڶ��̻߳����ﰲȫ��ִ��
	queue_type& m_queue;                //����tcp_message����Ϣ���У������ﱣ����Ϣ��ָ������ǿ���
	static object_pool_type m_msg_pool; //�ڴ��
	
	safe_atom<bool> m_bconnected;


	function_type m_connect_func;
	function_type m_disconnect_func;

};

typedef boost::shared_ptr<tcp_session> tcp_session_ptr; //����ָ�붨��

#endif