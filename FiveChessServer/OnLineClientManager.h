#ifndef _ONLINE_CLIENTMANAGER_H
#define _ONLINE_CLIENTMANAGER_H

#include "tcp_session.hpp"
#include "packet.h"

//单个在线用户的信息
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


//保存当前在线用户的相关信息
class OnLineClientManager
{
public:
	//维护两个列表，一个是根据名称进行管理；
	typedef std::map<std::string, OnLineClient> OnLineClients;

	//一个是根据组号进行管理
	//该处采用双重map进行管理，类似于二维数组
	typedef std::map<int, OnLineClients> GroupInfos;
public:
	OnLineClientManager();
	~OnLineClientManager();
public:
	bool AddOnLineClient(const OnLineClient& client);

	//根据键值查找
	bool FindOnLineClient(const std::string& strName);
	
	//根据会话端查找
	bool FindOnLineClient(tcp_session_ptr& session);

	//查找指定会话端的数据
	bool FindOnLineClient(tcp_session_ptr& session, OnLineClient& info);

	void RemoveClient(tcp_session_ptr& session);

	//根据名称更新在线用户的当前组号
	void UpdateOnLineClientGroupID(const std::string& strName, int nGroupID);

	//根据名称更新在线用户的游戏状态
	void UpdateOnLineClientGameState(const std::string& strName, GAMESTATE state);
	//////////////////////////////////////////////////////////////////////////

	//获取某组的玩家信息
	void GetGroupInfoByID(int nGroupID, OnLineClients& vClients);



	//维护分组相关信息
private:
	void AddGroupInfo(int nGroupID, const OnLineClient& info);

	//修改分组信息列表，主要是指修改指定客户端的组号信息
	void UpdateClientGroupInfo(int nNewGroupID, const OnLineClient& info);

	void RemoveClientFromGroupInfo(const std::string& strName);
public:
	OnLineClients& GetOnLineClients();

private:
	OnLineClients m_mpOnLineClient;

	GroupInfos    m_mpGroups;

};



#endif