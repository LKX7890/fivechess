#ifndef _SAFE_MAP_H
#define _SAFE_MAP_H

#include "safe_container.h"
#include <boost/unordered_map.hpp>


//对容器进行封装，增加线程安全
template<typename Key, typename Value>
//线程安全读写的hash_map
class  safe_map:public safe_container
{
	typedef boost::unordered_map<Key, Value> map_type; //使用散列表
public:
	typedef typename map_type::key_type    key_type;
	typedef typename map_type::mapped_type mapped_type;
	typedef typename map_type::value_type  value_type;

	typedef typename map_type::pointer pointer;
	typedef typename map_type::const_pointer const_pointer;

	typedef typename map_type::size_type size_type;
	typedef typename map_type::difference_type difference_type;

	typedef typename map_type::reference reference;
	typedef typename map_type::const_reference const_reference;
	typedef typename map_type::iterator iterator;
	typedef typename map_type::const_iterator const_iterator;

	typedef boost::optional<mapped_type> optional_mapped_type;

public:
	bool empty() const;
	size_type size() const;
	size_type max_size() const;
	iterator begin();
	const_iterator begin() const;
	iterator end();
	const_iterator end() const;
	bool insert(const key_type &key, const mapped_type &v);
	bool find(const key_type &k);
	size_type erase(const key_type &k);
	void clear();

	optional_mapped_type at(const key_type &k);
	const mapped_type &operator[](const key_type &k);
	void set(const key_type &k, const mapped_type &v);

	template<typename Func>
	void for_each(Func func);
private:
	map_type m_map;
	//mutable rw_mutex m_mutex;
};

template<typename Key, typename Value>
bool safe_map<Key, Value>::empty() const
{
	read_lock lock(m_mutex);
	return m_map.empty();
}


template<typename Key, typename Value>
typename safe_map<Key, Value>::size_type safe_map<Key, Value>::size() const
{
	read_lock lock(m_mutex);
	return m_map.size();
}

template<typename Key, typename Value>
typename safe_map<Key, Value>::size_type safe_map<Key, Value>::max_size() const
{
	read_lock lock(m_mutex);
	return m_map.max_size();
}

template<typename Key, typename Value>
typename safe_map<Key, Value>::iterator safe_map<Key, Value>::begin()
{
	read_lock lock(m_mutex);
	return m_map.begin();
}


template<typename Key, typename Value>
typename safe_map<Key, Value>::const_iterator safe_map<Key, Value>::begin() const
{
	read_lock lock(m_mutex);
	return m_map.begin();
}

template<typename Key, typename Value>
typename safe_map<Key, Value>::iterator safe_map<Key, Value>::end()
{
	read_lock lock(m_mutex);
	return m_map.end();
}

template<typename Key, typename Value>
typename safe_map<Key, Value>::const_iterator safe_map<Key, Value>::end() const
{
	read_lock lock(m_mutex);
	return m_map.end();
}


template<typename Key, typename Value>
bool safe_map<Key, Value>::insert(const key_type &key, const mapped_type &v)
{
	write_lock lock(m_mutex);
	return m_map.insert(value_type(key, v)).second;
}

template<typename Key, typename Value>
bool safe_map<Key, Value>::find(const key_type &k)
{
	read_lock lock(m_mutex);
	return m_map.find(k) != m_map.end();
}

template<typename Key, typename Value>
typename safe_map<Key, Value>::size_type safe_map<Key, Value>::erase(const key_type &k)
{
	write_lock lock(m_mutex);
	return m_map.erase(k);
}

template<typename Key, typename Value>
void safe_map<Key, Value>::clear()
{
	write_lock lock(m_mutex);
	return m_map.clear();
}

template<typename Key, typename Value>
typename safe_map<Key, Value>::optional_mapped_type safe_map<Key, Value>::at(const key_type &k)
{
	read_lock lock(m_mutex);
	if (m_map.find(k) != m_map.end())
	{
		return optional_mapped_type(m_map[k]);
	}
	return optional_mapped_type();
}

template<typename Key, typename Value>
const typename safe_map<Key, Value>::mapped_type &safe_map<Key, Value>::operator [](const key_type &k)
{
	read_lock lock(m_mutex);
	BOOST_ASSERT(m_map.find(k) != m_map.end());
	return m_map[k];
}

template<typename Key, typename Value>
void safe_map<Key, Value>::set(const key_type &k, const mapped_type &v)
{
	write_lock lock(m_mutex);
	m_map[k] = v;
}

template<typename Key, typename Value>
template<typename Func>
void safe_map<Key, Value>::for_each(Func func)
{
	read_lock lock(m_mutex);
	std::for_each(m_map.begin(), m_map.end(), func);
}

#endif