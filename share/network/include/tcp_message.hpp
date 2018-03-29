#ifndef _TCP_MESSAGE_H
#define _TCP_MESSAGE_H

//#include "dllexport.h"

#include "msg_def.hpp"
#include <boost/checked_delete.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/array.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/crc.hpp>
//#include "tcp_session.hpp"
//using namespace boost;

class tcp_session;
typedef boost::shared_ptr<tcp_session> tcp_session_ptr; //共享指针定义

class  tcp_message: boost::noncopyable
{
public:
	typedef boost::function<void(tcp_message *)> destroy_type;  //销毁器
	typedef char char_type;
	typedef boost::array<char_type, sizeof(msg_head)> head_data_type;
	typedef boost::array<char_type, MAX_MSG_SIZE> body_data_type;

public:
	template<typename Func>
	tcp_message(const tcp_session_ptr &s, Func func);
	tcp_message(const tcp_session_ptr &s);


	tcp_session_ptr get_session();

	void destroy();

	head_data_type &head_data();
	body_data_type &msg_data();

	msg_head* get_head();
	bool check_head();
	bool check_msg_crc();
	void set_msg_crc();
private:
	tcp_session_ptr m_session;
	destroy_type m_destroy;

	head_data_type m_head;  //消息头
	body_data_type m_msg;   //消息体

};

typedef tcp_message tcp_request;       //tcp请求消息
typedef tcp_message tcp_response;      //tcp响应消息 
typedef tcp_request* tcp_request_ptr;
typedef tcp_response* tcp_response_ptr;


#endif