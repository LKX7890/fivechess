//io_service
//1. ��ͬ��ģʽ�£� ������һ��IO��������io_service�ύ����io_service������ת����ϵͳ
//   ͬ���ĵȴ�����IO������ɺ󣬲���ϵͳ֪ͨio_service,Ȼ��io_service��������ظ�����
//2. ���첽ģʽ�£��趨�����ڻص�����ɴ�����
//3. io_service::run()-->����ǰ�����¼�����ѭ���������ȴ����еĲ�����ɲ������¼�

#ifndef _IO_SERVICE_POOL_H
#define _IO_SERVICE_POOL_H

#include "dllexport.h"

#include <algorithm>
#include <boost/assert.hpp>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/functional/factory.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include "thread_header.hpp"
//�ֶ���̵߳�io_service_pool,���е�ÿһ���߳��ﶼ������һ�������¼���������io_service��,
//�ɳ�ֶ��CPU������
class NETWORK_API io_service_pool:boost::noncopyable
{
public:
	typedef boost::asio::io_service ios_type;
	typedef boost::asio::io_service::work work_type;
	typedef boost::ptr_vector<ios_type>  io_services_type;
	typedef boost::ptr_vector<work_type> works_type;
public:
	explicit io_service_pool(int n = 4);
public:
	ios_type& get();
	void start();
	void run();
	void stop();
private:
	void init(int n);
private:
	io_services_type m_io_services;    //ָ�������洢
	works_type       m_works;          //��work��֤io_service��ת
	boost::thread_group m_threads;     //�̳߳�
	std::size_t   m_next_io_service;   //������ѯ�õ�io_service
};

#endif
