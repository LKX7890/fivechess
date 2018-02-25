#include "OnLineClientManager.h"

OnLineClientManager::OnLineClientManager()
{

}


OnLineClientManager::~OnLineClientManager()
{

}

bool OnLineClientManager::FindOnLineClient(tcp_session_ptr& session)
{
	OnLineClients::iterator iter = m_mpOnLineClient.begin();
	for (; iter != m_mpOnLineClient.end(); ++iter)
	{
		if (iter->second.session == session)
		{
			return true;
		}
	}
	return false;
}

void OnLineClientManager::RemoveClient(tcp_session_ptr& session)
{
	OnLineClients::iterator iter = m_mpOnLineClient.begin();
	for (; iter != m_mpOnLineClient.end(); ++iter)
	{
		if (iter->second.session == session)
		{
			//ͬʱ�ӷ�����Ϣ�б����Ƴ�
			RemoveClientFromGroupInfo(iter->second.m_strName);
			m_mpOnLineClient.erase(iter);
			break;
		}
	}
}


bool OnLineClientManager::FindOnLineClient(const std::string& strName)
{
	OnLineClients::iterator iter = m_mpOnLineClient.find(strName);
	if (iter != m_mpOnLineClient.end())
	{
		return true;
	}
	return false;
}


bool OnLineClientManager::FindOnLineClient(tcp_session_ptr& session, OnLineClient& info)
{
	OnLineClients::iterator iter = m_mpOnLineClient.begin();
	for (; iter != m_mpOnLineClient.end(); ++iter)
	{
		if (iter->second.session == session)
		{
			info = iter->second;
			return true;
		}
	}
	return false;
}



bool OnLineClientManager::AddOnLineClient(const OnLineClient& client)
{
	if (FindOnLineClient(client.m_strName))
	{
		return false;
	}
	m_mpOnLineClient.insert(std::make_pair(client.m_strName, client));

	//ͬʱ���·�����Ϣ����
	AddGroupInfo(client.m_nGroupID, client);
	return true;
}


OnLineClientManager::OnLineClients& OnLineClientManager::GetOnLineClients()
{
	return m_mpOnLineClient;
}

void OnLineClientManager::UpdateOnLineClientGroupID(const std::string& strName, 
													int nGroupID)
{
	OnLineClients::iterator iter = m_mpOnLineClient.begin();
	for (; iter != m_mpOnLineClient.end(); ++iter)
	{
		if (iter->second.m_strName == strName)
		{
			//�ȸ�������
			iter->second.m_nGroupID = nGroupID;
			
			//�ٸ����б�
			UpdateClientGroupInfo(nGroupID, iter->second);
			break;
		}
	}
}

void OnLineClientManager::UpdateOnLineClientGameState(const std::string& strName, GAMESTATE state)
{
	OnLineClients::iterator iter = m_mpOnLineClient.begin();
	for (; iter != m_mpOnLineClient.end(); ++iter)
	{
		if (iter->second.m_strName == strName)
		{
			//�ȸ�������
			iter->second.m_euGameState = state;

			//�ٸ����б�
			UpdateClientGroupInfo(iter->second.m_nGroupID, iter->second);
			break;
		}
	}
}


void OnLineClientManager::AddGroupInfo(int nGroupID, const OnLineClient& info)
{
	m_mpGroups[nGroupID].insert(std::make_pair(info.m_strName, info));
}

void OnLineClientManager::RemoveClientFromGroupInfo(const std::string& strName)
{
	GroupInfos::iterator iterGroup = m_mpGroups.begin();
	for (; iterGroup != m_mpGroups.end(); ++iterGroup)
	{
		OnLineClients::iterator iterLient = iterGroup->second.begin();
		for (; iterLient != iterGroup->second.end(); ++iterLient)
		{
			if (iterLient->second.m_strName == strName)
			{
				iterGroup->second.erase(iterLient);
				return;
			}
		}
	}
}

void OnLineClientManager::UpdateClientGroupInfo(int nNewGroupID, const OnLineClient& info)
{
	//���Ƴ�
	RemoveClientFromGroupInfo(info.m_strName);

	//����������б���
	AddGroupInfo(nNewGroupID, info);
}

void OnLineClientManager::GetGroupInfoByID(int nGroupID, OnLineClients& vClients)
{
	vClients.clear();
	GroupInfos::iterator iter = m_mpGroups.find(nGroupID);
	if (iter != m_mpGroups.end())
	{
		vClients = iter->second;
	}
}