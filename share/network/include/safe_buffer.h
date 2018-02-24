#ifndef _SAFE_BUFFER_H
#define _SAFE_BUFFER_H

#include <boost/thread.hpp>
#include <boost/noncopyable.hpp>

using namespace boost;

//��������������ͬ������ʵ������������--�����ߵĻ�����ģ��
template<typename T>
class  safe_buffer:public boost::noncopyable
{
public:
	typedef typename boost::mutex mutex_type;
	typedef typename boost::mutex::scoped_lock mutex_lock_type;
	typedef typename boost::condition_variable_any condition_type;
	typedef typename std::size_t size_type;

public:
	safe_buffer(size_type n);

public:
	virtual void put(const T &value){};
	virtual void get(T &value){};

	bool is_full();
	bool is_empty();

protected:
	mutex_type m_mutex;
	condition_type m_put_cond;    //д����������
	condition_type m_get_cond;    //��ȡ��������

	int m_read;                   //�����жϻ�������
	int m_capacity;
};

template<typename T>
safe_buffer<T>::safe_buffer(size_type n):m_read(0), m_capacity(n)
{

}


template<typename T>
bool safe_buffer<T>::is_full()
{
	return m_read == m_capacity;
}

template<typename T>
bool safe_buffer<T>::is_empty()
{
	return 0 == m_read;
}


#endif