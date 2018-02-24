#ifndef _SAFE_CONTAINER_H
#define _SAFE_CONTAINER_H

#include <algorithm>
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>
#include <boost/optional.hpp>
#include <boost/assert.hpp>

//封装线程安全的容器
class  safe_container:public boost::noncopyable
{
protected:
	typedef boost::shared_mutex rw_mutex;              //共享互斥量
	typedef boost::shared_lock<rw_mutex> read_lock;    //读锁定    
	typedef boost::unique_lock<rw_mutex> write_lock;   //写锁定
protected:
	mutable rw_mutex m_mutex;  //读写锁
};



#endif