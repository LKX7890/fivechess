#ifndef _PACKET_FACTORY_H
#define _PACKET_FACTORY_H

#include <boost/array.hpp>
#include <iostream>
#include <map>

#include "packet_type.h"
#include "msg_def.hpp"
#include <boost/archive/text_iarchive.hpp>
#include <boost/functional/factory.hpp>
#include "packet.h"

//using namespace boost;
//using namespace std;
class serial_packet;
class login_packet;

typedef char char_type;
typedef boost::array<char_type, MAX_MSG_SIZE> data_type;
typedef std::size_t size_type;


//定义函数指针
typedef serial_packet* (*packet_creator)(data_type&, size_type );

template<class Packet>
serial_packet* create_packet(data_type &data, size_type size)
{
	std::string strData(data.data(), size);
	stringstream ss(strData);
	boost::archive::text_iarchive ia(ss);
	Packet *pkt = boost::factory<Packet*>()();
	pkt->unserial(ia);
	return pkt;
}
class packet_factory
{
public:
	typedef std::map<packet_type, packet_creator> packet_creators;
	const packet_creators get_creators() const
	{
		return m_mpCreators;
	}

	template<class Packet>
	bool register_packet_type(packet_type type)
	{
		return m_mpCreators.insert(std::make_pair(type, create_packet<Packet>)).second;
	}

	serial_packet *generate_packet(packet_type type, data_type &data, size_type size)
	{
		packet_creators::iterator iter = m_mpCreators.find(type);
		if (iter != m_mpCreators.end())
		{
			return iter->second(data, size);
		}
		return NULL;
	}
private:
	packet_creators m_mpCreators;
};



#endif