#ifndef _ServerDataManager_H
#define _ServerDataManager_H

#include <boost/container/detail/singleton.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "ClientInfoManager.h"
#include "OnLineClientManager.h"
#include "MainApp.h"

typedef boost::shared_ptr<ClientInfoManager>   ClientInfoManagerPtr;
typedef boost::shared_ptr<OnLineClientManager> OnLineClientManagerPtr;
class ServerDataManager
{
public:
	ServerDataManager();
	~ServerDataManager();
public:
	void InitServerDataManager();
public:
	ClientInfoManagerPtr   GetClientInfoManagerPtr();
	OnLineClientManagerPtr GetOnLineClientManagerPtr();
public:
	ClientInfoManagerPtr    m_pClientInfoManager;
	OnLineClientManagerPtr  m_pOnLineClientManager;
};

//创建单件
typedef singleton_default<ServerDataManager> theData;


#endif