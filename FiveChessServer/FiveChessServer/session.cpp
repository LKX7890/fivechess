#include "session.hpp"

#include <iostream>
#include <boost/bind.hpp>  

Session::Session(boost::asio::io_service& _ioService)
	:socket_(_ioService) {
	memset(msg_, 0, sizeof(msg_));
}


Session::~Session(void)
{
}


void Session::start(void) {
	// �������ӳɹ��Ŀͻ��ˣ�����Ҫ����Ϣ��  
	char msg[256] = "001:Connect Succeed! Please tell me with 10 bytes, the total data and the size of each package, example:128 1024";
	boost::asio::async_write(socket_, boost::asio::buffer(msg, strlen(msg)),
		boost::bind(&Session::init_handler, shared_from_this(),
		boost::asio::placeholders::error));
}

boost::asio::ip::tcp::socket& Session::socket(void) {
	return socket_;
}


// ��һ��Э���  
void Session::init_handler(const boost::system::error_code& _error) {
	if (_error) {
		return;
	}
	// ��ȡ�ͻ��˷����� 10 bytes��ȷ���������Ĵ�С�Լ������ܴ�С  
	boost::asio::async_read(socket_, boost::asio::buffer(msg_, 10),
		boost::bind(&Session::analyse_handler, shared_from_this(),
		boost::asio::placeholders::error));

}

void Session::analyse_handler(const boost::system::error_code& _error) {
	if (_error) {
		return;
	}
	// ����Э�����ʽ  
	bool bflag = true;
	// ���������ʽ  

	// do something.  
	if (!bflag) {
		start();
		return;
	}

	// ��ʽ������Э�������  
	std::stringstream io(msg_);
	io >> maxSize_;
	io >> sumSize_;

	// ���ͽ���������Ϣ  
	char msg[REPLY_SIZE];
	sprintf_s(msg, "001:is ok, data remaining %d.", sumSize_);
	boost::asio::async_write(socket_, boost::asio::buffer(msg, REPLY_SIZE),
		boost::bind(&Session::write_handler, shared_from_this(),
		boost::asio::placeholders::error));
}


// ������ݴ���  
void Session::done_handler(const boost::system::error_code& _error) {
	if (_error) {
		return;
	}
	currentMsg_ += msg_;
	// ������Ϣ���ص�  
	if (!currentMsg_.empty() && callback_ != nullptr) {
		callback_(socket_.remote_endpoint().address().to_string(), currentMsg_);
		currentMsg_.clear();
	}
	memset(msg_, 0, sizeof(msg_));

	char msg[32] = "001:will done.";
	boost::asio::async_write(socket_, boost::asio::buffer(msg, REPLY_SIZE),
		boost::bind(&Session::init_handler, shared_from_this(),
		boost::asio::placeholders::error));
}

void Session::read_handler(const boost::system::error_code& _error, size_t _readSize) {
	if (_error) {
		return;
	}
	// ���ݴ���  
	currentMsg_ += msg_;
	if (currentMsg_.size() > 1024 * 512) {
		// ������Ϣ���ص�  
		if (callback_ != nullptr) {
			callback_(socket_.remote_endpoint().address().to_string(), currentMsg_);
			currentMsg_.clear();
		}
	}
	memset(msg_, 0, sizeof(msg_));

	// ���㵱ǰʣ����������  
	sumSize_ -= _readSize;

	// �������  
	if (0 > sumSize_) {
		done_handler(_error);
	}
	// ��������  
	else {
		char msg[REPLY_SIZE];
		sprintf_s(msg, "001:%d.", sumSize_);
		boost::asio::async_write(socket_, boost::asio::buffer(msg, REPLY_SIZE),
			boost::bind(&Session::write_handler, shared_from_this(),
			boost::asio::placeholders::error));

		std::cout << "send client recv succeed: " << msg << std::endl;
	}



}
void Session::write_handler(const boost::system::error_code& _error) {
	if (_error) {
		return;
	}

	boost::asio::async_read(socket_, boost::asio::buffer(msg_, maxSize_),
		boost::bind(&Session::read_handler, shared_from_this(),
		boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}