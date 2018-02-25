#ifndef _JOB_QUEUE_H
#define _JOB_QUEUE_H

#include <queue>
#include <boost/noncopyable.hpp>
#include <boost/utility/value_init.hpp>
#include <boost/concept_check.hpp>
#include <boost/thread.hpp>

//一个可用于多线程环境下的阻塞队列，可以用于生产者-消费者模式
template<typename Job>
class  job_queue:boost::noncopyable
{
public:
	typedef Job job_type;
	typedef std::deque<job_type> queue_type;
	typedef boost::mutex    mutex_type;
	typedef typename mutex_type::scoped_lock lock_type;
	typedef boost::condition_variable_any condition_type;

	BOOST_CONCEPT_ASSERT((boost::SGIAssignable<job_type>));
	BOOST_CONCEPT_ASSERT((boost::DefaultConstructible<job_type>));
public:
	job_queue();
public:
	void push(const job_type &x);
	job_type pop();
	void stop();
private:
	queue_type m_queue;
	mutex_type m_mutex;
	condition_type m_hasJob;
	bool   m_stop_flag;
};


template<typename Job>
job_queue<Job>::job_queue():m_stop_flag(false)
{
}

template<typename Job>
void job_queue<Job>::push(const job_type &x)
{
	lock_type lock(m_mutex);
	m_queue.push_back(x);
	m_hasJob.notify_one();
}


template<typename Job>
typename job_queue<Job>::job_type job_queue<Job>::pop()
{
	lock_type lock(m_mutex);
	while (m_queue.empty() && !m_stop_flag)
	{
		m_hasJob.wait(m_mutex);
	}

	if (m_stop_flag)
	{
		return boost::initialized_value;
	}

	if (m_queue.empty())
	{
		return boost::initialized_value;
	}
/*	BOOST_ASSERT(!m_queue.empty());*/

	job_type tmp = m_queue.front();
	m_queue.pop_front();
	return tmp;
}

template<typename Job>
void job_queue<Job>::stop()
{
	m_stop_flag = true;
	m_hasJob.notify_all();
}

#endif