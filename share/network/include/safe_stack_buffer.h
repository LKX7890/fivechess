#ifndef _SAFE_STACK_BUFFER_H
#define _SAFE_STACK_BUFFER_H

//基于"栈"结构的生产者消费者模型
#include "safe_buffer.h"
#include <stack>

template<typename T>
class  safe_stack_buffer:public safe_buffer<T>
{
public:
	safe_stack_buffer(size_type n);
private:
	typedef typename std::stack<T> buffer_type;
public:
	virtual void put(const T &value);
	virtual void get(T &value);
private:
	buffer_type m_buf;
};


template<typename T>
safe_stack_buffer<T>::safe_stack_buffer(size_type n):safe_buffer<T>(n)
{

}

template<typename T>
void safe_stack_buffer<T>::put(const T &value)
{
	{
		mutex_lock_type lock(m_mutex);
		while (is_full())   //缓冲区满
		{
			{
				mutex::scoped_lock lock(io_mu);
				std::cout<<"full waiting..."<<std::endl;
			}
			m_put_cond.wait(m_mutex); //条件变量等待
		}
		
		//条件满足，停止等待
		m_buf.push(value);
		++m_read;
	}
	//解除互斥量
	//通知可以读取数据
	m_get_cond.notify_one();
}



template<typename T>
void safe_stack_buffer<T>::get(T &value)
{
	{
		mutex_lock_type lock(m_mutex);
		while (is_empty())
		{
			{
				mutex::scoped_lock lock(io_mu);
			    std::cout<<"empty waiting..."<<std::endl;
			}
			m_get_cond.wait(m_mutex);
		}
		--m_read;
		value = m_buf.top();
		m_buf.pop();
	}
	m_put_cond.notify_one();
}




#endif