#ifndef _SAFE_VECTOR_H
#define _SAFE_VECTOR_H

//#include "dllexport.h"

#include "safe_container.h"

#include <vector>

template<typename T>
class  safe_vector:public safe_container
{
	typedef std::vector<T> vector_type;
public:
	typedef typename vector_type::value_type   value_type;
	typedef typename vector_type::pointer       poiner;
	typedef typename vector_type::const_pointer const_pointer;
	typedef typename vector_type::size_type size_type;
	typedef typename vector_type::reference reference;
	typedef typename vector_type::const_reference const_reference;
	typedef typename vector_type::iterator iterator;
	typedef typename vector_type::const_iterator const_iterator;
	typedef typename vector_type::reverse_iterator reverse_iterator;
	typedef typename vector_type::const_reverse_iterator const_reverse_iterator;

	typedef boost::optional<vector_type> optional_vector_type;
public:
	bool empty() const;
	size_type size() const;
	size_type max_size() const;
	iterator begin();
	const_iterator begin() const;
	iterator end();
	const_iterator end() const;
	bool find(const T &value);
	const T& operator[](const int k);
	void push_back(const T& value);

	template<typename Func>
	void for_each(Func func);

	void clear();
	void erase(const_iterator _where);

	bool remove(const T &value);
private:
	vector_type m_vector;
};

template<typename T>
bool safe_vector<T>::empty() const
{
	read_lock lock(m_mutex);
	return m_vector.empty();
}

template<typename T>
typename safe_vector<T>::size_type safe_vector<T>::size() const
{
	read_lock lock(m_mutex);
	return m_vector.size();
}


template<typename T>
typename safe_vector<T>::size_type safe_vector<T>::max_size() const
{
	read_lock lock(m_mutex);
	return m_vector.max_size();
}

template<typename T>
typename safe_vector<T>::iterator safe_vector<T>::begin()
{
	read_lock lock(m_mutex);
	return m_vector.begin();
}

template<typename T>
typename safe_vector<T>::const_iterator safe_vector<T>::begin() const
{
	read_lock lock(m_mutex);
	return m_vector.begin();
}


template<typename T>
typename safe_vector<T>::iterator safe_vector<T>::end()
{
	read_lock lock(m_mutex);
	return m_vector.end();
}

template<typename T>
typename safe_vector<T>::const_iterator safe_vector<T>::end() const
{
	read_lock lock(m_mutex);
	return m_vector.end();
}


template<typename T>
const T &safe_vector<T>::operator [](const int k)
{
	read_lock lock(m_mutex);
	return m_vector[k];
}

template<typename T>
void safe_vector<T>::push_back(const T& value)
{
	write_lock lock(m_mutex);
	m_vector.push_back(value);
}

template<typename T>
template<typename Func>
void safe_vector<T>::for_each(Func func)
{
	read_lock lock(m_mutex);
	std::for_each(m_vector.begin(), m_vector.end(), func);
}

template<typename T>
void safe_vector<T>::clear()
{
	write_lock lock(m_mutex);
	m_vector.clear();
}

template<typename T>
void safe_vector<T>::erase(const_iterator _where)
{
	write_lock lock(m_mutex);
	m_vector.erase(_where);
}

template<typename T>
bool safe_vector<T>::find(const T &value)
{
	read_lock lock(m_mutex);
	vector_type::const_iterator iter = m_vector.begin();
	for (; iter != m_vector.end(); ++iter)
	{
		if (*iter == value)
		{
			return true;
		}
	}
	return false;
}

template<typename T>
bool safe_vector<T>::remove(const T &value)
{
	write_lock lock(m_mutex);
	vector_type::iterator iter = m_vector.begin();
	for (; iter != m_vector.end(); ++iter)
	{
		if (value == *iter)
		{
			m_vector.erase(iter);
			return true;
		}
	}
	return false;
}

#endif