#ifndef _INTDEF_H
#define _INTDEF_H

#include <boost/cstdint.hpp>

//��ȷ��������Ķ���

using boost::int8_t;
using boost::uint8_t;
using boost::int16_t;
using boost::uint16_t;
using boost::int32_t;
using boost::uint32_t;
using boost::int64_t;
using boost::uint64_t;

typedef boost::uint8_t byte_t;
typedef boost::uint8_t uchar_t;
typedef unsigned short ushort_t;
typedef unsigned int   uint_t;
typedef unsigned long  ulong_t;

typedef boost::uint16_t word_t;
typedef boost::uint32_t dword_t;


struct msg_head
{
	uint32_t type;   //��Ϣ����
	uint32_t size;   //��Ϣ���С
	uint32_t chksum; //CRCУ��
};

#define MAX_MSG_SIZE 1024  //��Ϣ����󳤶�
#define SEND_BUFFER  4096  //���÷�����Ϣ�Ļ�����Ϊ4096
#define PORT_NUM     9999
#endif