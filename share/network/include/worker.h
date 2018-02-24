#ifndef _WORKER_H
#define _WORKER_H

#include <boost/assert.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <boost/thread.hpp>

#include "job_queue.hpp"

#include <iostream>

//������job_queue�ϵ������ߣ�����ʹ��һ�������̴߳�������
//�����̵߳���Ҫ�����ǶԶ����е����ݽ��д���
template<typename Queue>
class  worker
{
public:
	typedef Queue queue_type;
	typedef typename Queue::job_type job_type;
	typedef boost::function<bool(job_type &)> func_type; //����������
public:
	template<typename Func>
	worker(queue_type &q, Func func, int n = 1);

	worker(queue_type &q, int n = 1);

	template<typename Func>
	void set_workerFunc(Func func)
	{
		m_func = func;
	}

public:
	void start();

	template<typename Func>
	void start(Func func);

	void run();
	void stop();
private:
	void do_work();

private:
	queue_type &m_queue;
	func_type  m_func;
	int m_nThreadNum;
	boost::thread_group m_threads;
};


template<typename Queue>
template<typename Func>
worker<Queue>::worker(queue_type &q, Func func, int n):
m_queue(q), m_func(func), m_nThreadNum(n)
{
	if (m_nThreadNum < 1)
	{
		m_nThreadNum = 1;
	}
}


template<typename Queue>
worker<Queue>::worker(queue_type &q, int n):
m_queue(q), m_nThreadNum(n)
{
	if (m_nThreadNum < 1)
	{
		m_nThreadNum = 1;
	}
}

template<typename Queue>
void worker<Queue>::start()
{
	if (!m_func)
	{
		return;
	}

	if (m_threads.size() > 0) //�Ѿ��������߳�
	{
		return;
	}

	for (int i = 0; i < m_nThreadNum; ++i)
	{
		m_threads.create_thread(boost::bind(&worker::do_work, this));
	}
}

template<typename Queue>
template<typename Func>
void worker<Queue>::start(Func func)
{
	m_func = func;
	start();
}


template<typename Queue>
void worker<Queue>::run()
{
	start();
	m_threads.join_all();
}



template<typename Queue>
void worker<Queue>::do_work()    //ѭ������������
{
	for (;;)
	{
		//��������û������ʱ���߳�һֱ���ڵȴ�״̬
		job_type job = m_queue.pop();  //�Ӷ�����ȡ������

		if (!m_func)
		{
			break;
		}
		else
		{
			m_func(job);
		}
	}
}


template<typename Queue>
void worker<Queue>::stop()
{
	m_func = 0;
	m_queue.stop();
}

#endif