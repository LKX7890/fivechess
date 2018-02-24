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
	//�������
	void OnPacket(tcp_session_ptr& session, add_to_group_packet* packet);
	void OnPacket(tcp_session_ptr& session, leave_to_group_packet* packet);

	//������Ϸ״̬�İ�����
	void OnPacket(tcp_session_ptr& session, control_game_packet* packet);
public:
	//�����пͻ��˶Ͽ�ʱ���߼�
	void OnClientDisconnected(tcp_session_ptr& session);

private:
	//�����пͻ��˷��������
	void SendPacketToAllOnLineClient(serial_packet& packet);
private:
	//���������ͻ��˸����������
	void NotifyOtherClient(const std::string& strName);
	void NotifyOtherClientExceptSession(const std::string& strName,
		                                tcp_session_ptr& session,
										serial_packet& packet);

	void SendAllClientsInfo(tcp_session_ptr& session);

	//�����ĳ����ҵĻỰ�˷�������
	void SendPacketToGroupExceptSession(int nGroupID, serial_packet& packet, tcp_session_ptr& session);
};

#endif