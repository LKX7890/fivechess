#ifndef __SESSION_HPP_
#define __SESSION_HPP_

#include <string>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

#define REPLY_SIZE (32)  

// �Ự�࣬�̳д�֧��ͳһ����ָ��
class Session : public boost::enable_shared_from_this < Session >
{

public:

	typedef void pSessionCallback(std::string, std::string);

public:

	Session(boost::asio::io_service& _ioService);
	virtual ~Session(void);

	void start(void);

	void setCallback(pSessionCallback* _callback) { callback_ = _callback; }

	// socket ʵ��  
	boost::asio::ip::tcp::socket& socket(void);

private:

	// ��һ��Э���  
	void init_handler(const boost::system::error_code& _error);

	// ����Э���  
	void analyse_handler(const boost::system::error_code& _error);

	// ������ݴ���󴥷�����β����  
	void done_handler(const boost::system::error_code& _error);

	// ��ȡ�ɹ��󴥷��ĺ���  
	void read_handler(const boost::system::error_code& _error, size_t _readSize);

	// д����ɺ󴥷��ĺ���  
	void write_handler(const boost::system::error_code& _error);

private:
	// ��ʱ��Ϣ������  
	char msg_[1024];
	std::string currentMsg_;
	// ����������  
	int sumSize_;
	// �������ݰ���С  
	unsigned int maxSize_;
	// socket���  
	boost::asio::ip::tcp::socket socket_;
	// �ص�����ָ��
	pSessionCallback* callback_;				

};
#endif
