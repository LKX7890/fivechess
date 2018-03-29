#ifndef _PACKET_TYPE_H
#define _PACKET_TYPE_H

//�������Ҫ���������л��������ͣ�ͨ����������ʶ���
//�¼ӵİ�Ӧ���ڴ˴������Ӧ�İ�����

typedef enum empacket_type
{
	PACKET_DEFAULT = 0xffff,
	PACKET_HEART = 0x8000,
	
	PACKET_LOGIN,
	PACKET_ACK_LOGIN,

	PACKET_LOGOUT,
	PACKET_ACK_LOGOUT,

	PACKET_REGISTER,
	PACKET_ACK_REGISTER,

	PACKET_ADD_CLIENT,     
	PACKET_REMOVE_CLIENT,

	PACKET_CLIENTINFO,

	PACKET_ADDTOGROUP,     //ĳһ���ͻ��˼������

	PACKET_LEAVEGROUP,     //ĳһ���ͻ����뿪����

	PACKET_CONTROL_GAME,   //�൱�ڿ�����Ϸ״̬

	PACKET_ROLEINFO,

	PACKET_REALTIME_ROLEINFO, //��ɫ��ʵʱ��Ϣ

	PACKET_GROUP_ROLEINFO,

	PACKET_CLIENT_GAME_STATUS,

	PACKET_ALL_CLIENTINFO,
	
	PACKET_DISCONNECT,
}packet_type;




#endif