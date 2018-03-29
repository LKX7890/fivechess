#ifndef _TCP_BUFFER_H
#define _TCP_BUFFER_H

//#include "dllexport.h"

#include "include_header/thread_header.hpp"
#include <boost/config/suffix.hpp>
#include <boost/cast.hpp>

//该类用于tcp通信时的数据读写
class  tcp_buffer
{
public:
	typedef std::size_t size_type;
	typedef boost::asio::streambuf streambuf_type;
	typedef streambuf_type::const_buffers_type const_buffers_type;
	typedef streambuf_type::mutable_buffers_type mutable_buffers_type;
public:
	//准备固定大小的缓冲区用于接收数据
	mutable_buffers_type prepare(size_type n = BUF_SIZE)
	{
		return m_buf.prepare(n);
	}

	//获取接收到的n个字节
	void retrieve(size_type n)
	{
		return m_buf.commit(n);
	}

	//获得可读的字节数量
	size_type size() const
	{
		return m_buf.size();
	}

	//查看可读的字节
	const char *peek() const
	{
		return boost::asio::buffer_cast<const char *>(m_buf.data());
	}

	//////////////////////////////////////////////////////////////////////////
	//发送数据接口
	void append(const void *data, size_type len)
	{
		m_buf.sputn(static_cast<const char *>(data), boost::numeric_cast<std::streamsize>(len));
	}

	const_buffers_type data() const
	{
		return m_buf.data();
	}

	//收发完数据后调用该函数，告诉缓冲区消费的字节数
	void consume(size_type n)
	{
		m_buf.consume(n);
	}
private:
	BOOST_STATIC_CONSTANT(size_type, BUF_SIZE = 512);
	streambuf_type m_buf;
};

#endif