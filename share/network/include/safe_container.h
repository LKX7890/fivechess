#ifndef _SAFE_CONTAINER_H
#define _SAFE_CONTAINER_H

#include <algorithm>
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>
#include <boost/optional.hpp>
#include <boost/assert.hpp>

//��װ�̰߳�ȫ������
class  safe_container:public boost::noncopyable
{
protected:
	typedef boost::shared_mutex rw_mutex;              //��������
	typedef boost::shared_lock<rw_mutex> read_lock;    //������    
	typedef boost::unique_lock<rw_mutex> write_lock;   //д����
protected:
	mutable rw_mutex m_mutex;  //��д��
};



#endif