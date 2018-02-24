#ifndef _MAINAPP_H
#define _MAINAPP_H

#include <boost/container/detail/singleton.hpp>
using boost::container::container_detail::singleton_default;

#include "MServer.h"

class MainApp
{
public:
	MainApp();
	~MainApp();
public:
	void InitAppInstance();
	void SendPacket(tcp_session_ptr& session, serial_packet& packet);
private:
	void StartServer();
private:
	boost::shared_ptr<MServer> m_pServer;
};

//创建单件
typedef singleton_default<MainApp> theApp;


#endif
