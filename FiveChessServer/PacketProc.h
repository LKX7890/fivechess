#ifndef _PACKETPROC_H
#define _PACKETPROC_H

#include "MServer.h"

class login_packet;
class PacketProc
{
public:
	PacketProc();
	~PacketProc();
public:
	void OnPacket(tcp_session_ptr& session, login_packet* packet);
	void OnPacket(tcp_session_ptr& session, registerinfo_packet* packet); 

	//////////////////////////////////////////////////////////////////////////
	//分组管理
	void OnPacket(tcp_session_ptr& session, add_to_group_packet* packet);
	void OnPacket(tcp_session_ptr& session, leave_to_group_packet* packet);

	//控制游戏状态的包处理
	void OnPacket(tcp_session_ptr& session, control_game_packet* packet);
public:
	//处理有客户端断开时的逻辑
	void OnClientDisconnected(tcp_session_ptr& session);

private:
	//向所有客户端发送网络包
	void SendPacketToAllOnLineClient(serial_packet& packet);
private:
	//告诉其它客户端该玩家已连接
	void NotifyOtherClient(const std::string& strName);
	void NotifyOtherClientExceptSession(const std::string& strName,
		                                tcp_session_ptr& session,
										serial_packet& packet);

	void SendAllClientsInfo(tcp_session_ptr& session);

	//向除了某个玩家的会话端发送数据
	void SendPacketToGroupExceptSession(int nGroupID, serial_packet& packet, tcp_session_ptr& session);
};

#endif