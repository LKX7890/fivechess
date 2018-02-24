#ifndef _ONLINE_CLIENTMANAGER_H
#define _ONLINE_CLIENTMANAGER_H

#include "tcp_session.hpp"
#include "packet.h"

//���������û�����Ϣ
class OnLineClient
{
public:
	OnLineClient()
	:m_nGroupID(-1), m_euGameState(GAME_NO_PREPARE)
	{

	}
public:
	std::string m_strName;
	std::string m_strPhoto;
	int         m_nGroupID;
	tcp_session_ptr session;
	GAMESTATE   m_euGameState;
};


//���浱ǰ�����û��������Ϣ
class OnLineClientManager
{
public:
	//ά�������б�һ���Ǹ������ƽ��й���
	typedef std::map<std::string, OnLineClient> OnLineClients;

	//һ���Ǹ�����Ž��й���
	//�ô�����˫��map���й��������ڶ�ά����
	typedef std::map<int, OnLineClients> GroupInfos;
public:
	OnLineClientManager();
	~OnLineClientManager();
public:
	bool AddOnLineClient(const OnLineClient& client);

	//���ݼ�ֵ����
	bool FindOnLineClient(const std::string& strName);
	
	//���ݻỰ�˲���
	bool FindOnLineClient(tcp_session_ptr& session);

	//����ָ���Ự�˵�����
	bool FindOnLineClient(tcp_session_ptr& session, OnLineClient& info);

	void RemoveClient(tcp_session_ptr& session);

	//�������Ƹ��������û��ĵ�ǰ���
	void UpdateOnLineClientGroupID(const std::string& strName, int nGroupID);

	//�������Ƹ��������û�����Ϸ״̬
	void UpdateOnLineClientGameState(const std::string& strName, GAMESTATE state);
	//////////////////////////////////////////////////////////////////////////

	//��ȡĳ��������Ϣ
	void GetGroupInfoByID(int nGroupID, OnLineClients& vClients);



	//ά�����������Ϣ
private:
	void AddGroupInfo(int nGroupID, const OnLineClient& info);

	//�޸ķ�����Ϣ�б���Ҫ��ָ�޸�ָ���ͻ��˵������Ϣ
	void UpdateClientGroupInfo(int nNewGroupID, const OnLineClient& info);

	void RemoveClientFromGroupInfo(const std::string& strName);
public:
	OnLineClients& GetOnLineClients();

private:
	OnLineClients m_mpOnLineClient;

	GroupInfos    m_mpGroups;

};



#endif