#ifndef _CODE_H
#define _CODE_H

#include <vector>
#include <string>
using namespace std;

//定义一些回复码，表示处理的情况
typedef enum enumack_code
{
	CODE_LOGIN_SUCCESS = 1,    //登录成功
	CODE_REGISTER_SUCCESS,     //注册成功
	CODE_REGISTER_EXIST,       //注册失败，该用户已存在
	CODE_USERNAME_NOT_EXIST,   //用户名错误或不存在
	CODE_PASSWORD_WRONG,       //密码错误
	CODE_USER_LOGIN_AGAIN,     //该用户已登录

}ack_code;


inline std::string GetAckCodeString(ack_code code)
{
	std::string strCode;
	switch (code)
	{
	case CODE_LOGIN_SUCCESS:
		strCode = "登录成功......";
		break;
	case CODE_REGISTER_SUCCESS:
		strCode = "注册成功......";
		break;
	case CODE_REGISTER_EXIST:
		strCode = "该用户已存在......";
		break;
	case CODE_PASSWORD_WRONG:
		strCode = "密码错误......";
		break;
	case CODE_USERNAME_NOT_EXIST:
		strCode = "用户名不存在......";
		break;
	case CODE_USER_LOGIN_AGAIN:
		strCode = "该用户已登录......";
		break;
	default:
		strCode = "未知错误......";
		break;
	}

	return strCode;
}




#endif
