#include "ClientInfoManager.h"

#include "xmlOperator.h"

ClientInfoManager::ClientInfoManager()
{
	XmlOperator xml;
	xml.read(*this, "config.xml");
}

ClientInfoManager::~ClientInfoManager()
{

}

void ClientInfoManager::SavaClientInfo()
{
	XmlOperator xml;
	xml.write(*this, "config.xml");
}


bool ClientInfoManager::AddClientInfo(const std::string& strName,
									  const std::string& strPwd,
									  const std::string& strPhoto)
{
	if (FindClientInfo(strName))
	{
		return false;
	}
	clientinfo info;
	info.m_strName  = strName;
	info.m_strPwd   = strPwd;
	info.m_strPhoto = strPhoto;
	m_vClientInfo.push_back(info);
	return true;
}


bool ClientInfoManager::FindClientInfo(const std::string& strName)
{
	infos::iterator iter = m_vClientInfo.begin();
	for (; iter != m_vClientInfo.end(); ++iter)
	{
		if (iter->m_strName == strName)
		{
			return true;
		}
	}
	return false;
}

bool ClientInfoManager::FindClientInfo(const std::string& strName, clientinfo& info)
{
	infos::iterator iter = m_vClientInfo.begin();
	for (; iter != m_vClientInfo.end(); ++iter)
	{
		if (iter->m_strName == strName)
		{
			info = *iter;
			return true;
		}
	}
	return false;
}

