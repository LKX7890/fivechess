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

//该类主要是处理服务器与客户端共同需要处理的一些操作接口
//将共有的操作属性封装在该类中
//该类应该只处理实际的物理连接，具体的连接的含义应在子类中体现
class NETWORK_API net_comm
{
public:
	friend class tcp_comm;
	net_comm(int sec_check = 1);
public:
	typedef job_queue<tcp_request_ptr> req_queue_type;     //接收消息队列
	typedef worker<req_queue_type> req_worker_type;        //处理已收到的消息
	typedef job_queue<tcp_response_ptr> resp_queue_type;   //发送消息队列
	typedef worker<resp_queue_type>   resp_worker_type;    //处理待发送的消息
	typedef scheduler schedule_type;
	typedef safe_vector<std::string> vector_type;
	
	typedef safe_vector<tcp_session_ptr> vector_session_type; //管理连接会话端
	//////////////////////////////////////////////////////////////////////////
	//以下接口主要是供外部调用，提供对网络及数据的控制
public:
	void start();                  //启动工作线程
	req_queue_type &recv_queue();  //接收消息队列的访问函数
protected:
	
	//处理有会话端连接上
	virtual bool proc_connect(tcp_session_ptr &session) = 0;

	//处理收到网络的包
	virtual bool proc_packet(tcp_session_ptr &session, serial_packet *packet) = 0;

	//处理会话端断开
	virtual bool proc_disconnect(tcp_session_ptr &session) = 0;
	
	//注册要处理的包类型
	virtual void proc_registerpacket() = 0;

	//探测会话端状态
	virtual void check_session_status();

protected:

	bool session_connected(tcp_session_ptr &session);
	bool session_disconnected(tcp_session_ptr &session);

	//向指定的会话端发送数据
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
	//消息处理回调函数
	bool process_msg(tcp_request_ptr &req);
	bool send_msg(tcp_response_ptr &resp);
	//根据序列化的包构建会话数据
	void generate_session_data(tcp_response_ptr &resp, serial_packet &packet);

protected:
	req_queue_type   m_recvq;           //接收消息的队列
	resp_queue_type  m_sendq;           //待发送的消息队列
	req_worker_type  m_req_worker;      //处理已接收消息的工作线程
	resp_worker_type m_resp_worker;     //处理待发送消息的工作线程      
	packet_factory   m_packetfactory;   //包生成工厂
	schedule_type    m_schedule;        //检测网络连接状态
	vector_session_type m_tcp_sessions; //该列表不保存重复的元素
};



#endif