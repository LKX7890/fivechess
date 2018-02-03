#ifndef __SERVER_HPP_
#define __SERVER_HPP_

#include <string>
#include <boost/asio.hpp>  
#include <boost/shared_ptr.hpp> 

class Session;
class Server {
private:
	// �Ự - ����ָ��  
	typedef boost::shared_ptr<Session> session_ptr;
public:
	Server(boost::asio::io_service &_ioService, boost::asio::ip::tcp::endpoint &_endpoint);
	virtual ~Server(void);
	// ����  
	void start(void);
	// �첽  
	void run(void);
private:
	// ���ݵ����ӿ�  
	void callback_session(std::string _fromIp, std::string _info);
	// �Ự����  
	void accept_handler(session_ptr _chatSession, const boost::system::error_code& _error);
private:
	boost::asio::io_service &ioService_;			// io���񣬳����˲���ϵͳ�ĳ���
	boost::asio::ip::tcp::acceptor acceptor_;		// ͨ�� endpoint ������ɳ�ʼ��Ϊָ����Э��Ͷ˿ں� - ���ڵȴ�������PC���������
};
#endif
