#ifndef _CODE_H
#define _CODE_H

#include <vector>
#include <string>
using namespace std;

//����һЩ�ظ��룬��ʾ��������
typedef enum enumack_code
{
	CODE_LOGIN_SUCCESS = 1,    //��¼�ɹ�
	CODE_REGISTER_SUCCESS,     //ע��ɹ�
	CODE_REGISTER_EXIST,       //ע��ʧ�ܣ����û��Ѵ���
	CODE_USERNAME_NOT_EXIST,   //�û�������򲻴���
	CODE_PASSWORD_WRONG,       //�������
	CODE_USER_LOGIN_AGAIN,     //���û��ѵ�¼

}ack_code;


inline std::string GetAckCodeString(ack_code code)
{
	std::string strCode;
	switch (code)
	{
	case CODE_LOGIN_SUCCESS:
		strCode = "��¼�ɹ�......";
		break;
	case CODE_REGISTER_SUCCESS:
		strCode = "ע��ɹ�......";
		break;
	case CODE_REGISTER_EXIST:
		strCode = "���û��Ѵ���......";
		break;
	case CODE_PASSWORD_WRONG:
		strCode = "�������......";
		break;
	case CODE_USERNAME_NOT_EXIST:
		strCode = "�û���������......";
		break;
	case CODE_USER_LOGIN_AGAIN:
		strCode = "���û��ѵ�¼......";
		break;
	default:
		strCode = "δ֪����......";
		break;
	}

	return strCode;
}




#endif
