#ifndef _SERIAL_PACKET_H
#define _SERIAL_PACKET_H

#pragma warning(disable:4308)

#include <boost/serialization/serialization.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <boost/serialization/export.hpp>
#include <boost/iostreams/stream.hpp>


#include <iostream>
#include <strstream>
#include <sstream>

#include "packet_type.h"

using namespace std;

using namespace boost::archive;
using namespace boost::iostreams;

//�ú��������Ϊ���ṩ���л��Ľӿڣ�����̳���serial_packet
//������Ҫ���л�����Ӧ����Ӹú꣬���⣬�κ����л��İ���Ӧ��
//�ڹ��캯������ȷָ������Ψһ����

#define _SERIALIZATION_PACKET()\
	public:\
	virtual void serial(text_oarchive &oa)\
    {\
	  oa << *this;\
    }\
	virtual void unserial(text_iarchive &ia)\
    {\
	  ia >> *this;\
    }\
	public:\
	int get_packet_type()\
    {\
	  return static_cast<int>(m_packet_type);\
    }\
	protected:\
	friend boost::serialization::access;\
	
	


//ÿ���һ���̳��Ը���İ�����Ҫʵ�����²���
//1.���_SERIALIZATION_PACKET()��
//2.ʵ�ֻ���Ĺ��캯��
//3.��Ҫ���ģ�庯��serialize����������ʵ�����ݵ����л�����
class serial_packet
{
	_SERIALIZATION_PACKET()
protected:
	serial_packet(packet_type pkt_type):m_packet_type(pkt_type){}
protected:
	template<typename Archive>
	void serialize( Archive & ar, const unsigned int version)
	{
		ar & m_packet_type;
	}
protected:
	packet_type m_packet_type;
};


#endif