#ifndef _SAFE_ATOM_H
#define _SAFE_ATOM_H

template<typename T>
class safe_atom
{
	typedef shared_mutex mutex_t;
public:
	safe_atom(T v);
	void setValue(const T &v);
	void getValue(T &v);
private:
	mutex_t m_mu;
	T value;
};

template<typename T>
safe_atom<T>::safe_atom(T v):value(v)
{
}

template<typename T>
void safe_atom<T>::setValue(const T &v)
{
	unique_lock<shared_mutex> lock(m_mu);
	value = v;
}

template<typename T>
void safe_atom<T>::getValue(T &v)				
{
	shared_lock<shared_mutex> lock(m_mu);
	v = value;
}

#endif