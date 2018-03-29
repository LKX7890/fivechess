//io_service
//1. 在同步模式下， 程序发起一个IO操作，向io_service提交请求，io_service将操作转交给系统
//   同步的等待。当IO操作完成后，操作系统通知io_service,然后io_service将结果发回给程序
//2. 在异步模式下，需定义用于回调的完成处理函数
//3. io_service::run()-->启动前摄器事件处理循环，阻塞等待所有的操作完成并分派事件

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
//持多个线程的io_service_pool,其中的每一个线程里都运行着一个并发事件处理器（io_service）,
//可充分多核CPU的能力
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
	io_services_type m_io_services;    //指针容器存储
	works_type       m_works;          //用work保证io_service运转
	boost::thread_group m_threads;     //线程池
	std::size_t   m_next_io_service;   //用于轮询得到io_service
};

#endif
