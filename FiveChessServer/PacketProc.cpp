#include "PacketProc.h"
#include "packet.h"
#include <iostream>
#include "ServerDataManager.h"
#include "MainApp.h"

PacketProc::PacketProc()
{

}


PacketProc::~PacketProc()
{

}


//�����û���¼��
void PacketProc::OnPacket(tcp_session_ptr &session, login_packet *packet)
{
	ack_login_packet ack_packet;

	//�ж��û����������Ƿ���ȷ
	//1.�����û�
	ClientInfoManagerPtr ptr = theData::instance().GetClientInfoManagerPtr();
	
	clientinfo info;

	bool bFind = ptr->FindClientInfo(packet->m_strName, info);

	bool bLoginSuccessful = false;

	OnLineClientManagerPtr onLinePtr = theData::instance().GetOnLineClientManagerPtr();

	if (bFind)
	{


		//�������
		bool bRight = (info.m_strPwd == packet->m_strPwd);
		
		//������ȷ
		if (bRight)
		{
			//�����û��Ƿ������ߣ�������ߣ��͵�¼ʧ�ܣ������¼�ɹ�
			
			if (onLinePtr->FindOnLineClient(packet->m_strName))
			{
				ack_packet.m_code = CODE_USER_LOGIN_AGAIN;
			}
			else
			{
				//��¼�ɹ�
				ack_packet.m_code = CODE_LOGIN_SUCCESS;
				ack_packet.m_strName = packet->m_strName;

				//��ȡ����ҵ�ͷ����Ϣ
				ack_packet.m_strPhoto = info.m_strPhoto;
				bLoginSuccessful = true;
			}
		}
		//���벻��ȷ
		else
		{
			ack_packet.m_code = CODE_PASSWORD_WRONG;
		}

	}
	//���û�������
	else
	{
		ack_packet.m_code = CODE_USERNAME_NOT_EXIST;
	}
	theApp::instance().SendPacket(session, ack_packet);


	//��¼�ɹ��Ժ�
	if (bLoginSuccessful)
	{
		//֪ͨ�������߿ͻ��˸��û��ѵ�¼
		add_client_packet add_packet;
		add_packet.m_strName = info.m_strName;
		add_packet.m_strPhoto = info.m_strPhoto;
		add_packet.m_nGroupID = -1;
		SendPacketToAllOnLineClient(add_packet);


		//ͬʱ����������ҵ����ݷ����������
		SendAllClientsInfo(session);

		OnLineClient client;
		client.m_strName  = info.m_strName;
		client.m_strPhoto = info.m_strPhoto;
		client.session    = session;
		client.m_nGroupID = -1;
		//��ӵ������б�
		onLinePtr->AddOnLineClient(client);
	}
}


//�����û�ע��ɹ�ʱ����������Ҫ����д�������ļ���
void PacketProc::OnPacket(tcp_session_ptr &session, registerinfo_packet *packet)
{
	//1.�����û�
	ClientInfoManagerPtr ptr = theData::instance().GetClientInfoManagerPtr();

	bool bFind = ptr->FindClientInfo(packet->m_strName);

	ack_register_packet ack_packet;
	bool bRegisterSuccessful = false;
	if (bFind)
	{
		ack_packet.m_code = CODE_REGISTER_EXIST;    //ע��ʧ��
	}
	else
	{
		bRegisterSuccessful = true;
		ack_packet.m_code = CODE_REGISTER_SUCCESS; //ע��ɹ�
	}

	theApp::instance().SendPacket(session, ack_packet);

	//ע��ɹ��󣬼��뵽�û���Ϣ
	if (bRegisterSuccessful)
	{
		ptr->AddClientInfo(packet->m_strName, packet->m_strPwd, packet->m_strPhotoName);
		ptr->SavaClientInfo();
	}
}

//���������ͻ��˸�����ѵ�¼
void PacketProc::NotifyOtherClient(const std::string& strName)
{
	add_client_packet packet;
	packet.m_strName = strName;

	OnLineClientManagerPtr onLinePtr = theData::instance().GetOnLineClientManagerPtr();

	OnLineClientManager::OnLineClients clients = onLinePtr->GetOnLineClients();

	OnLineClientManager::OnLineClients::iterator iter = clients.begin();
	for (; iter != clients.end(); ++iter)
	{
		if (iter->second.session)
		{
			theApp::instance().SendPacket(iter->second.session, packet);
		}
	}
}


//��������ҵ���Ϣ���͸������
void PacketProc::SendAllClientsInfo(tcp_session_ptr& session)
{
	OnLineClientManagerPtr onLinePtr = theData::instance().GetOnLineClientManagerPtr();

	OnLineClientManager::OnLineClients clients = onLinePtr->GetOnLineClients();

	OnLineClientManager::OnLineClients::iterator iter = clients.begin();

	all_client_packet allInfoPacket;
	clientinfo_packet clientInfo;

	for (; iter != clients.end(); ++iter)
	{
		clientInfo.m_strName  = iter->first;
		clientInfo.m_strPhoto = iter->second.m_strPhoto;
		clientInfo.m_nGroupId = iter->second.m_nGroupID;
		allInfoPacket.m_vClientInfo.push_back(clientInfo);
	}

	theApp::instance().SendPacket(session, allInfoPacket);
}


void PacketProc::SendPacketToAllOnLineClient(serial_packet& packet)
{
	OnLineClientManagerPtr onLinePtr = theData::instance().GetOnLineClientManagerPtr();

	OnLineClientManager::OnLineClients clientSessions = onLinePtr->GetOnLineClients();

	OnLineClientManager::OnLineClients::iterator iter = clientSessions.begin();
	for (; iter != clientSessions.end(); ++iter)
	{
		theApp::instance().SendPacket(iter->second.session, packet);
	}
}





void PacketProc::OnClientDisconnected(tcp_session_ptr& session)
{
	//��ĳһ���ͻ��˶Ͽ����Ӻ���Ҫ�Ƴ����߹������е���Ӧ����

    OnLineClientManagerPtr onLinePtr = theData::instance().GetOnLineClientManagerPtr();
	
	//step 1 ��ȡ�ÿͻ��˵���Ϣ
	OnLineClient info;

	if (!onLinePtr->FindOnLineClient(session, info))
	{
		return;
	}

	//step 2 �Ƴ��ûỰ��
	onLinePtr->RemoveClient(session);

	//step 3 �������ͻ��˷����Ƴ��Ự�˵�����
	remove_client_packet packet;
	packet.m_strName  = info.m_strName;
	packet.m_nGroupID = info.m_nGroupID;
	SendPacketToAllOnLineClient(packet);

}





//////////////////////////////////////////////////////////////////////////
//�������
//���пͻ��˼������ʱ�����������Ŀͻ���
void PacketProc::OnPacket(tcp_session_ptr& session, add_to_group_packet* packet)
{
	//���·����������߿ͻ��˵������Ϣ
	OnLineClientManagerPtr onLinePtr = theData::instance().GetOnLineClientManagerPtr();
	onLinePtr->UpdateOnLineClientGroupID(packet->m_strName, packet->m_nNewGroupID);
	//ת��
	NotifyOtherClientExceptSession(packet->m_strName, session, *packet);
}



void PacketProc::OnPacket(tcp_session_ptr& session, leave_to_group_packet* packet)
{
	//���·����������߿ͻ��˵������Ϣ
	OnLineClientManagerPtr onLinePtr = theData::instance().GetOnLineClientManagerPtr();
	onLinePtr->UpdateOnLineClientGroupID(packet->m_strName, packet->m_nOldGroupId);
	//ת��
	NotifyOtherClientExceptSession(packet->m_strName, session, *packet);
}


void PacketProc::NotifyOtherClientExceptSession(const std::string& strName,
												tcp_session_ptr& session,
												serial_packet& packet)
{

	OnLineClientManagerPtr onLinePtr = theData::instance().GetOnLineClientManagerPtr();

	OnLineClientManager::OnLineClients clients = onLinePtr->GetOnLineClients();

	OnLineClientManager::OnLineClients::iterator iter = clients.begin();
	for (; iter != clients.end(); ++iter)
	{
		if (iter->second.session && (iter->second.session != session))
		{
			theApp::instance().SendPacket(iter->second.session, packet);
		}
	}
}

void PacketProc::OnPacket(tcp_session_ptr& session, control_game_packet* packet)
{
	//�յ���Ϸ״̬���ư��Ժ󣬸������߿ͻ��˵���Ϸ״̬
	OnLineClientManagerPtr onLinePtr = theData::instance().GetOnLineClientManagerPtr();
	onLinePtr->UpdateOnLineClientGameState(packet->m_strName, packet->m_euState);

	//�������ҷ���׼����Ϸ�Ĳ�������ô����������ڵ���
	//���������һ������Ѿ���׼����Ϸ��״̬����ôͬʱ����������ҷ��Ϳ�ʼ��Ϸ������
	//����÷����Ѿ���ʼ��Ϸ����ô����ֱ�Ӽ�����Ϸ
	if (GAME_PREPARE == packet->m_euState)
	{
		OnLineClientManager::OnLineClients vClients;
		onLinePtr->GetGroupInfoByID(packet->m_nGroupId, vClients);

		

		//�����ǰ����ֻ��һ����Ҽ��룬��ʱ���ظ�
		if (vClients.size() < 2)
		{
			return;
		}


		//������÷����������ҷ���״̬
		client_game_status_packet status_packet;
		status_packet.m_strName  = packet->m_strName;
		status_packet.m_nGroupId = packet->m_nGroupId;
		status_packet.m_euState  = packet->m_euState;
		SendPacketToGroupExceptSession(status_packet.m_nGroupId, status_packet, session);

		//�����ǰ�������������������ϵ���Ҽ��룬
		//��ô����û�п�ʼ����ҷ��Ϳ��Կ�ʼ��Ϸ������
		OnLineClientManager::OnLineClients::iterator iter = vClients.begin();

		//������׼����ʼ���������
		OnLineClientManager::OnLineClients tempClients;
		for (; iter != vClients.end(); ++iter)
		{
			if (iter->second.m_euGameState == GAME_PREPARE ||
				iter->second.m_euGameState == GAME_START)
			{
				tempClients.insert(*iter);
			}
		}

		if (tempClients.size() < 2)
		{
			return;
		}

		for (iter = tempClients.begin(); iter != tempClients.end(); ++iter)
		{
			//����׼������ҷ��Ϳ�ʼ��Ϸ�Ĳ���
			if (iter->second.m_euGameState == GAME_PREPARE)
			{
				control_game_packet ack_packet;
				ack_packet.m_strName  = iter->second.m_strName;
				ack_packet.m_nGroupId = iter->second.m_nGroupID;
				ack_packet.m_euState  = GAME_START;

				//��һ�θ���״̬
				onLinePtr->UpdateOnLineClientGameState(iter->second.m_strName, GAME_START);

				theApp::instance().SendPacket(iter->second.session, ack_packet);
			}
		}
	}
}


//��ĳ����ҷ��鷢�Ͱ�
void PacketProc::SendPacketToGroupExceptSession(int nGroupID,
												serial_packet& packet,
												tcp_session_ptr& session)
{
	OnLineClientManagerPtr onLinePtr = theData::instance().GetOnLineClientManagerPtr();
	OnLineClientManager::OnLineClients vClients;
	onLinePtr->GetGroupInfoByID(nGroupID, vClients);

	OnLineClientManager::OnLineClients::iterator iter = vClients.begin();
	for (; iter != vClients.end(); ++iter)
	{
		if (iter->second.session != session)
		{
			theApp::instance().SendPacket(iter->second.session, packet);
		}
	}
}