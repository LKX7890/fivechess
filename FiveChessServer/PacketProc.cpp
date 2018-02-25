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


//处理用户登录包
void PacketProc::OnPacket(tcp_session_ptr &session, login_packet *packet)
{
	ack_login_packet ack_packet;

	//判断用户名和密码是否正确
	//1.查找用户
	ClientInfoManagerPtr ptr = theData::instance().GetClientInfoManagerPtr();
	
	clientinfo info;

	bool bFind = ptr->FindClientInfo(packet->m_strName, info);

	bool bLoginSuccessful = false;

	OnLineClientManagerPtr onLinePtr = theData::instance().GetOnLineClientManagerPtr();

	if (bFind)
	{


		//检查密码
		bool bRight = (info.m_strPwd == packet->m_strPwd);
		
		//密码正确
		if (bRight)
		{
			//检查该用户是否已在线，如果在线，就登录失败，否则登录成功
			
			if (onLinePtr->FindOnLineClient(packet->m_strName))
			{
				ack_packet.m_code = CODE_USER_LOGIN_AGAIN;
			}
			else
			{
				//登录成功
				ack_packet.m_code = CODE_LOGIN_SUCCESS;
				ack_packet.m_strName = packet->m_strName;

				//获取该玩家的头像信息
				ack_packet.m_strPhoto = info.m_strPhoto;
				bLoginSuccessful = true;
			}
		}
		//密码不正确
		else
		{
			ack_packet.m_code = CODE_PASSWORD_WRONG;
		}

	}
	//该用户不存在
	else
	{
		ack_packet.m_code = CODE_USERNAME_NOT_EXIST;
	}
	theApp::instance().SendPacket(session, ack_packet);


	//登录成功以后
	if (bLoginSuccessful)
	{
		//通知其它在线客户端该用户已登录
		add_client_packet add_packet;
		add_packet.m_strName = info.m_strName;
		add_packet.m_strPhoto = info.m_strPhoto;
		add_packet.m_nGroupID = -1;
		SendPacketToAllOnLineClient(add_packet);


		//同时将该其它玩家的数据发送至该玩家
		SendAllClientsInfo(session);

		OnLineClient client;
		client.m_strName  = info.m_strName;
		client.m_strPhoto = info.m_strPhoto;
		client.session    = session;
		client.m_nGroupID = -1;
		//添加到在线列表
		onLinePtr->AddOnLineClient(client);
	}
}


//当有用户注册成功时，服务器需要将其写在配置文件中
void PacketProc::OnPacket(tcp_session_ptr &session, registerinfo_packet *packet)
{
	//1.查找用户
	ClientInfoManagerPtr ptr = theData::instance().GetClientInfoManagerPtr();

	bool bFind = ptr->FindClientInfo(packet->m_strName);

	ack_register_packet ack_packet;
	bool bRegisterSuccessful = false;
	if (bFind)
	{
		ack_packet.m_code = CODE_REGISTER_EXIST;    //注册失败
	}
	else
	{
		bRegisterSuccessful = true;
		ack_packet.m_code = CODE_REGISTER_SUCCESS; //注册成功
	}

	theApp::instance().SendPacket(session, ack_packet);

	//注册成功后，加入到用户信息
	if (bRegisterSuccessful)
	{
		ptr->AddClientInfo(packet->m_strName, packet->m_strPwd, packet->m_strPhotoName);
		ptr->SavaClientInfo();
	}
}

//告诉其它客户端该玩家已登录
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


//将其它玩家的信息发送给该玩家
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
	//当某一个客户端断开连接后，需要移除在线管理器中的相应数据

    OnLineClientManagerPtr onLinePtr = theData::instance().GetOnLineClientManagerPtr();
	
	//step 1 获取该客户端的信息
	OnLineClient info;

	if (!onLinePtr->FindOnLineClient(session, info))
	{
		return;
	}

	//step 2 移除该会话端
	onLinePtr->RemoveClient(session);

	//step 3 向其它客户端发送移除会话端的数据
	remove_client_packet packet;
	packet.m_strName  = info.m_strName;
	packet.m_nGroupID = info.m_nGroupID;
	SendPacketToAllOnLineClient(packet);

}





//////////////////////////////////////////////////////////////////////////
//分组管理
//当有客户端加入分组时，告诉其它的客户端
void PacketProc::OnPacket(tcp_session_ptr& session, add_to_group_packet* packet)
{
	//更新服务器端在线客户端的组号信息
	OnLineClientManagerPtr onLinePtr = theData::instance().GetOnLineClientManagerPtr();
	onLinePtr->UpdateOnLineClientGroupID(packet->m_strName, packet->m_nNewGroupID);
	//转发
	NotifyOtherClientExceptSession(packet->m_strName, session, *packet);
}



void PacketProc::OnPacket(tcp_session_ptr& session, leave_to_group_packet* packet)
{
	//更新服务器端在线客户端的组号信息
	OnLineClientManagerPtr onLinePtr = theData::instance().GetOnLineClientManagerPtr();
	onLinePtr->UpdateOnLineClientGroupID(packet->m_strName, packet->m_nOldGroupId);
	//转发
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
	//收到游戏状态控制包以后，更新在线客户端的游戏状态
	OnLineClientManagerPtr onLinePtr = theData::instance().GetOnLineClientManagerPtr();
	onLinePtr->UpdateOnLineClientGameState(packet->m_strName, packet->m_euState);

	//如果该玩家发起准备游戏的操作，那么检查该玩家所在的组
	//如果组内有一个玩家已经是准备游戏的状态，那么同时给这两个玩家发送开始游戏的命令
	//如果该分组已经开始游戏，那么可以直接加入游戏
	if (GAME_PREPARE == packet->m_euState)
	{
		OnLineClientManager::OnLineClients vClients;
		onLinePtr->GetGroupInfoByID(packet->m_nGroupId, vClients);

		

		//如果当前分组只有一个玩家加入，暂时不回复
		if (vClients.size() < 2)
		{
			return;
		}


		//首先向该分组的其它玩家发送状态
		client_game_status_packet status_packet;
		status_packet.m_strName  = packet->m_strName;
		status_packet.m_nGroupId = packet->m_nGroupId;
		status_packet.m_euState  = packet->m_euState;
		SendPacketToGroupExceptSession(status_packet.m_nGroupId, status_packet, session);

		//如果当前分组有两个或两个以上的玩家加入，
		//那么就向没有开始的玩家发送可以开始游戏的命令
		OnLineClientManager::OnLineClients::iterator iter = vClients.begin();

		//保存已准备或开始的玩家数量
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
			//向已准备的玩家发送开始游戏的操作
			if (iter->second.m_euGameState == GAME_PREPARE)
			{
				control_game_packet ack_packet;
				ack_packet.m_strName  = iter->second.m_strName;
				ack_packet.m_nGroupId = iter->second.m_nGroupID;
				ack_packet.m_euState  = GAME_START;

				//再一次更新状态
				onLinePtr->UpdateOnLineClientGameState(iter->second.m_strName, GAME_START);

				theApp::instance().SendPacket(iter->second.session, ack_packet);
			}
		}
	}
}


//向某个玩家分组发送包
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