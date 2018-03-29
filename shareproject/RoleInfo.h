#ifndef _ROLEINFO_H
#define _ROLEINFO_H

//�����ɫ����Ϣ
#include <string>
#include <vector>
#include <map>

using namespace std;

class RoleInfo
{
public:
	RoleInfo()
	{
		m_nFrame = 40;
		m_nCurrAnimation = 1;
		m_nLifeValue = 100;
	}
public:
	int m_xPos;
	int m_yPos;
	int m_zPos;
	int m_nDir;
	int m_nSpeed;
	int m_nFrame;     
	int m_nCurrAnimation;  //��ǰ�Ķ���
	int m_nLifeValue;      //����ֵ 
	std::string m_strName;
	int         m_nGroupID;
};


#endif
