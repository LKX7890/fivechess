#include "MainApp.h"
#include "ServerDataManager.h"

MainApp::MainApp()
{

}

MainApp::~MainApp()
{

}

void MainApp::StartServer()
{
	if (!m_pServer)
	{
		m_pServer = boost::make_shared<MServer>(PORT_NUM);
		m_pServer->Run();
	}
}

void MainApp::InitAppInstance()
{
	theData::instance().InitServerDataManager();

	StartServer();
}

void MainApp::SendPacket(tcp_session_ptr& session, serial_packet& packet)
{
	if (m_pServer)
	{
		m_pServer->SendPacket(session, packet);
	}
}