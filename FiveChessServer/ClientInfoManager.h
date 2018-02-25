#ifndef _USERINFO_H
#define _USERINFO_H

#pragma warning(disable:4308)
#include <map>
#include "xmlOperator.h"
#include "packet.h"

//��Ҫ����Ŀͻ�����Ϣ
class clientinfo
{
private:
	friend boost::serialization::access;
	template<typename Archive>
	void serialize( Archive & ar, const unsigned int version)
	{
		XML_PAIR("UserName", m_strName);
		XML_PAIR("UserPassWord", m_strPwd);
		XML_PAIR("UserPhoto", m_strPhoto);
	}
public:
	std::string m_strName;
	std::string m_strPwd;
	std::string m_strPhoto;
};


class ClientInfoManager
{
public:
	typedef std::vector<clientinfo> infos;
public:
	ClientInfoManager();
	~ClientInfoManager();
public:
	bool AddClientInfo(const std::string& strName, const std::string& strPwd, const std::string& strPhoto);
	bool FindClientInfo(const std::string& strName);
	bool FindClientInfo(const std::string& strName, clientinfo& info);
	void SavaClientInfo();
	//֧�����л�����
public:
	template<typename Archive>
	void xml_serialize(Archive &ar)
	{
		XML_PAIR("AllClientInfo", m_vClientInfo);
	}
private:
	infos m_vClientInfo;
};




#endif