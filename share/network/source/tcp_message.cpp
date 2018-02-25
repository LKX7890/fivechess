#include "tcp_message.hpp"


template<typename Func>
tcp_message::tcp_message(const tcp_session_ptr &s, Func func):
m_session(s), m_destroy(func)
{

}


tcp_message::tcp_message(const tcp_session_ptr &s):
m_session(s)
{

}


tcp_session_ptr tcp_message::get_session()
{
	return m_session;
}


void tcp_message::destroy()
{
	if (m_destroy)
	{
		m_destroy(this);
	}
	else
	{
		boost::checked_delete(this);
	}
}

tcp_message::head_data_type &tcp_message::head_data()
{
	return m_head;
}
tcp_message::body_data_type &tcp_message::msg_data()
{
	return m_msg;
}

msg_head* tcp_message::get_head()
{
	return reinterpret_cast<msg_head *>(m_head.data());
}

//简单地检查消息头是否正确
bool tcp_message::check_head()
{
	return (get_head()->size < MAX_MSG_SIZE);
}

//检查消息体的crc校验
bool tcp_message::check_msg_crc()
{
	boost::crc_32_type crc32;
	crc32.process_bytes(&m_msg[0], get_head()->size);
	//std::cout<<"get_head()->chksum:"<<get_head()->chksum<<std::endl;
	//std::cout<<"crc32.checksum():"<<crc32.checksum()<<std::endl;

	return get_head()->chksum == crc32.checksum();
}

//在消息头里设置消息体的crc校验
void tcp_message::set_msg_crc()
{
	boost::crc_32_type crc32;
	crc32.process_bytes(&m_msg[0], get_head()->size);
	get_head()->chksum = crc32.checksum();
}


