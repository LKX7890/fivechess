#ifndef _INTDEF_H
#define _INTDEF_H

#include <boost/cstdint.hpp>

//精确宽度整数的定义

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
	uint32_t type;   //消息类型
	uint32_t size;   //消息体大小
	uint32_t chksum; //CRC校验
};

#define MAX_MSG_SIZE 1024  //消息体最大长度
#define SEND_BUFFER  4096  //设置发送消息的缓冲区为4096
#define PORT_NUM     9999
#endif