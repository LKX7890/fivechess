#ifndef _TCP_BUFFER_H
#define _TCP_BUFFER_H

#include "dllexport.h"

#include "include_header/thread_header.hpp"
#include <boost/config/suffix.hpp>
#include <boost/cast.hpp>

//��������tcpͨ��ʱ�����ݶ�д
class  tcp_buffer
{
public:
	typedef std::size_t size_type;
	typedef boost::asio::streambuf streambuf_type;
	typedef streambuf_type::const_buffers_type const_buffers_type;
	typedef streambuf_type::mutable_buffers_type mutable_buffers_type;
public:
	//׼���̶���С�Ļ��������ڽ�������
	mutable_buffers_type prepare(size_type n = BUF_SIZE)
	{
		return m_buf.prepare(n);
	}

	//��ȡ���յ���n���ֽ�
	void retrieve(size_type n)
	{
		return m_buf.commit(n);
	}

	//��ÿɶ����ֽ�����
	size_type size() const
	{
		return m_buf.size();
	}

	//�鿴�ɶ����ֽ�
	const char *peek() const
	{
		return boost::asio::buffer_cast<const char *>(m_buf.data());
	}

	//////////////////////////////////////////////////////////////////////////
	//�������ݽӿ�
	void append(const void *data, size_type len)
	{
		m_buf.sputn(static_cast<const char *>(data), boost::numeric_cast<std::streamsize>(len));
	}

	const_buffers_type data() const
	{
		return m_buf.data();
	}

	//�շ������ݺ���øú��������߻��������ѵ��ֽ���
	void consume(size_type n)
	{
		m_buf.consume(n);
	}
private:
	BOOST_STATIC_CONSTANT(size_type, BUF_SIZE = 512);
	streambuf_type m_buf;
};

#endif