#ifndef _SERIAL_PACKET_H
#define _SERIAL_PACKET_H

#pragma warning(disable:4308)

//#include <boost/serialization/serialization.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

//#include <boost/serialization/export.hpp>
#include <boost/iostreams/stream.hpp>


#include <iostream>
#include <strstream>
#include <sstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "packet_type.h"

//using namespace std;

//using namespace boost::archive;
//using namespace boost::iostreams;

//该宏的作用是为类提供序列化的接口，如果继承自serial_packet
//的类需要序列化，就应该添加该宏，另外，任何序列化的包都应该
//在构造函数中明确指出包的唯一类型

#define _SERIALIZATION_PACKET()\
	public:\
	virtual void serial(boost::archive::text_oarchive &oa)\
    {\
	  oa << *this;\
    }\
	virtual void unserial(boost::archive::text_iarchive &ia)\
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
	
	


//每添加一个继承自该类的包，需要实现以下操作
//1.添加_SERIALIZATION_PACKET()宏
//2.实现基类的构造函数
//3.需要添加模板函数serialize，并在其中实现数据的序列化操作
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