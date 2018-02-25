#ifndef _MSERVER_H
#define _MSERVER_H

class PacketProc;

#include "ServerNetwork.h"
#include "packet.h"

class MServer:public ServerNetwork
{
public:
	MServer(int nPort);
	~MServer();
public:
	//向指定的会话 端发送数据
	void SendPacket(tcp_session_ptr& session, serial_packet& packet);

protected:
	bool proc_connect(tcp_session_ptr &session);
	bool proc_disconnect(tcp_session_ptr &session);
	bool proc_packet(tcp_session_ptr &session, serial_packet *packet);
	void proc_registerpacket();
private:
	boost::shared_ptr<PacketProc> m_pPacketProc;
};

#endif