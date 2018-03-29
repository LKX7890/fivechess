#ifndef _LOGIN_PACKET_H
#define _LOGIN_PACKET_H

#include "serial_packet.h"
#include <boost/serialization/vector.hpp>
//using namespace boost;
#include "ack_code.h"
#include "RoleInfo.h"


//////////////////////////////////////////////////////////////////////////
//��������
class heart_packet:public serial_packet
{
	_SERIALIZATION_PACKET()
public:
	heart_packet():serial_packet(PACKET_HEART){}
private:
	template<class Archive>
	void serialize( Archive & ar, const unsigned int version)
	{
		ar & m_nHour & m_nMin & m_nSec & m_nMilSec;
	}
public:
	int m_nHour;
	int m_nMin;
	int m_nSec;
	int m_nMilSec;
};

//////////////////////////////////////////////////////////////////////////
//��¼��
class login_packet:public serial_packet
{
	_SERIALIZATION_PACKET()
public:
	login_packet():serial_packet(PACKET_LOGIN){}
private:
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & m_strName & m_strPwd;
	}
public:
	std::string m_strName;
	std::string m_strPwd;
};


//////////////////////////////////////////////////////////////////////////
//��¼�ظ���
class ack_login_packet:public serial_packet
{
	_SERIALIZATION_PACKET()
public:
	ack_login_packet():serial_packet(PACKET_ACK_LOGIN){}
private:
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & m_code & m_strName & m_strPhoto;
	}
public:
	std::string m_strName;
	std::string m_strPhoto;
	ack_code    m_code;
};


//////////////////////////////////////////////////////////////////////////
//ע���
class registerinfo_packet:public serial_packet
{
	_SERIALIZATION_PACKET()
public:
	registerinfo_packet():serial_packet(PACKET_REGISTER){}
private:
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & m_strName & m_strPwd & m_strPhotoName;
	}
public:
	std::string m_strName;
	std::string m_strPwd;
	std::string m_strPhotoName;
};


//////////////////////////////////////////////////////////////////////////
//ע��ظ���
class ack_register_packet:public serial_packet
{
	_SERIALIZATION_PACKET()
public:
	ack_register_packet():serial_packet(PACKET_ACK_REGISTER){}
private:
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & m_code;
	}
public:
	ack_code m_code;
};


//ע��
class logout_packet: public serial_packet
{
	_SERIALIZATION_PACKET()
public:
	logout_packet():serial_packet(PACKET_LOGOUT){}
private:
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & m_strName;
	}
public:
	std::string m_strName;
};

//////////////////////////////////////////////////////////////////////////
//֪ͨ�������������Ҽ���
class add_client_packet: public serial_packet
{
	_SERIALIZATION_PACKET()
public:
	add_client_packet():serial_packet(PACKET_ADD_CLIENT){}
private:
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & m_strName & m_strPhoto & m_nGroupID;
	}
public:
	std::string m_strName;
	std::string m_strPhoto;
	int         m_nGroupID;
};


//////////////////////////////////////////////////////////////////////////
////֪ͨ�������������˳�
class remove_client_packet: public serial_packet
{
	_SERIALIZATION_PACKET()
public:
	remove_client_packet():serial_packet(PACKET_REMOVE_CLIENT){}
private:
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & m_strName & m_nGroupID;
	}
public:
	std::string m_strName;
	int m_nGroupID;
};

//////////////////////////////////////////////////////////////////////////
////ĳһ����ҵ���Ϣ
class clientinfo_packet: public serial_packet
{
	_SERIALIZATION_PACKET()
public:
	clientinfo_packet():serial_packet(PACKET_CLIENTINFO){}
private:
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & m_strName & m_strPhoto & m_nGroupId;
	}
public:
	std::string m_strName;
	std::string m_strPhoto;
	int         m_nGroupId;
};


//��������б�
class all_client_packet: public serial_packet
{
	_SERIALIZATION_PACKET()
public:
	all_client_packet():serial_packet(PACKET_ALL_CLIENTINFO){}
private:
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & m_vClientInfo;
	}
public:
	std::vector<clientinfo_packet> m_vClientInfo;
};

//////////////////////////////////////////////////////////////////////////
////ĳһ����Ҽ������
class add_to_group_packet: public serial_packet
{
	_SERIALIZATION_PACKET()
public:
	add_to_group_packet():serial_packet(PACKET_ADDTOGROUP){}
private:
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & m_strName & m_nOldGroupId & m_nNewGroupID;
	}
public:
	std::string m_strName;
	int         m_nOldGroupId;
	int         m_nNewGroupID;
};



class leave_to_group_packet: public serial_packet
{
	_SERIALIZATION_PACKET()
public:
	leave_to_group_packet():serial_packet(PACKET_LEAVEGROUP){}
private:
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & m_strName & m_nOldGroupId;
	}
public:
	std::string m_strName;
	int         m_nOldGroupId;
};



//////////////////////////////////////////////////////////////////////////
//������Ϸ״̬
typedef enum enumGAMESTATE
{
	GAME_NO_PREPARE, //��ʼ��Ϊû��׼��������Ϸ��״̬
	GAME_PREPARE,
	GAME_START,
	GAME_RUNNING,
	GAME_END
}GAMESTATE;

//����׼��������Ϸ�İ�
class control_game_packet: public serial_packet
{
	_SERIALIZATION_PACKET()
public:
	control_game_packet():serial_packet(PACKET_CONTROL_GAME){}
private:
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & m_strName & m_nGroupId & m_euState;
	}
public:
	std::string m_strName;
	int         m_nGroupId;
	GAMESTATE   m_euState;
};


class client_game_status_packet: public serial_packet
{
	_SERIALIZATION_PACKET()
public:
	client_game_status_packet():serial_packet(PACKET_CLIENT_GAME_STATUS){}
private:
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & m_strName & m_nGroupId & m_euState;
	}
public:
	std::string m_strName;
	int         m_nGroupId;
	GAMESTATE   m_euState;
};

//logic_1
class roleinfo_packet: public serial_packet
{
	_SERIALIZATION_PACKET()
public:
	roleinfo_packet():serial_packet(PACKET_ROLEINFO){}
private:
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & m_info.m_xPos & m_info.m_yPos & m_info.m_zPos & m_info.m_nDir & m_info.m_nSpeed;
		ar & m_info.m_nFrame & m_info.m_nCurrAnimation & m_info.m_nLifeValue;
		ar & m_info.m_strName & m_info.m_nGroupID;
	}
public:
	RoleInfo m_info;
};

//logic_1
class group_roleinfo_packet: public serial_packet
{
	_SERIALIZATION_PACKET()
public:
	group_roleinfo_packet():serial_packet(PACKET_GROUP_ROLEINFO){}
private:
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & m_vroleInfo;
	}
public:
	std::vector<roleinfo_packet> m_vroleInfo;
};


//logic_1
class realtime_roleinfo_packet: public serial_packet
{
	_SERIALIZATION_PACKET()
public:
	realtime_roleinfo_packet():serial_packet(PACKET_REALTIME_ROLEINFO){}
private:
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & m_roleInfo;
	}
public:
	roleinfo_packet m_roleInfo;
};

class msg_test :public serial_packet
{
	_SERIALIZATION_PACKET()
public:
	msg_test() :serial_packet(PACKET_TEST){}
private:
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & m_str;
	}
public:
	std::string m_str;
};
#endif