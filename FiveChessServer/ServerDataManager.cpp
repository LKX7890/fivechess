#include "ServerDataManager.h"
#include "ClientInfoManager.h"

ServerDataManager::ServerDataManager()
{

}


ServerDataManager::~ServerDataManager()
{

}


void ServerDataManager::InitServerDataManager()
{
	if (!m_pClientInfoManager)
	{
		m_pClientInfoManager = boost::make_shared<ClientInfoManager>();
	}
	if (!m_pOnLineClientManager)
	{
		m_pOnLineClientManager = boost::make_shared<OnLineClientManager>();
	}
}

ClientInfoManagerPtr ServerDataManager::GetClientInfoManagerPtr()
{
	if (!m_pClientInfoManager)
	{
		m_pClientInfoManager = boost::make_shared<ClientInfoManager>();
	}
	return m_pClientInfoManager;
}
 

OnLineClientManagerPtr ServerDataManager::GetOnLineClientManagerPtr()
{
	if (!m_pOnLineClientManager)
	{
		m_pOnLineClientManager = boost::make_shared<OnLineClientManager>();
	}
	return m_pOnLineClientManager;
}