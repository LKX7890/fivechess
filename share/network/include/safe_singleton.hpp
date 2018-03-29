#ifndef _SAFE_SINGLETON_H
#define _SAFE_SINGLETON_H

//#include "dllexport.h"

#include <boost/noncopyable.hpp>
#include <boost/thread/once.hpp>

template<typename T>
class  safe_singleton: boost noncopyable
{
public: 
	typedef T real_type;
	static real_type &instance();
private:
	static real_type &init();

private:
	static boost::once_flag m_flag;
};

template<typename T>
boost::once_flag safe_singleton<T>::m_flag = BOOST_ONCE_INIT;

template<typename T>
safe_singleton<T>::real_type safe_singleton<T>::instance()
{
	boost::call_once(m_flag, init);
	return init();
}

template<typename T>
safe_singleton<T>::real_type safe_singleton<T>::init()
{
	static real_type obj;
	return obj;
}



#endif