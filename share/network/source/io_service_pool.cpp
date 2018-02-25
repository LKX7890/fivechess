#include "io_service_pool.hpp"

io_service_pool::io_service_pool(int n):m_next_io_service(0)
{
	//��֤������һ���̱߳�����
	if (n <= 0)
	{
		n = 1;
	}
	init(n);
}


void io_service_pool::init(int n)
{
	for (int i = 0; i < n; ++i)
	{
		m_io_services.push_back(boost::factory<ios_type*>()());
		m_works.push_back(boost::factory<work_type*>()(m_io_services.back()));
	}
}


//��ѯ��ȡio_service
io_service_pool::ios_type &io_service_pool::get()
{
	if (m_next_io_service >= m_io_services.size())
	{
		m_next_io_service = 0;
	}
	return m_io_services[m_next_io_service++];
}

//�����¼�ѭ����������
void io_service_pool::start()
{
	if (m_threads.size() > 0)
	{
		return;
	}

	BOOST_FOREACH(ios_type &ios, m_io_services)
	{
		//ʹ���߳�ִ��io_service::run()�����¼�����ѭ��
		m_threads.create_thread(boost::bind(&ios_type::run, boost::ref(ios)));
	}
}


//����
void io_service_pool::run()
{
	start();
	m_threads.join_all();  //�����ȴ�
}

void io_service_pool::stop()
{
	m_works.clear();
	std::for_each(m_io_services.begin(), 
		          m_io_services.end(), 
				  boost::bind(&ios_type::stop, _1));
}