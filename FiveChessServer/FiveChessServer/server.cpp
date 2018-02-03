#include <boost/bind.hpp>  

#include "server.hpp"  
#include "session.hpp"  


Server::Server(boost::asio::io_service &_ioService, boost::asio::ip::tcp::endpoint &_endpoint) : ioService_(_ioService), acceptor_(_ioService, _endpoint) {
	start();
}


Server::~Server(void) {
}

void Server::start(void) {
	session_ptr new_chat_session(new Session(ioService_));

	// �첽������������
	acceptor_.async_accept(new_chat_session->socket(),
		boost::bind(&Server::accept_handler, this, new_chat_session,
		boost::asio::placeholders::error));
}

void Server::run(void) {
	// �÷���������֮�󣬻ص�����Żᱻ����
	ioService_.run();
}

void Server::callback_session(std::string /*_fromIp*/, std::string /*_info*/) {
	return;
}

// ��һ��PC��ͼ����һ������ʱ��accept_handler() ���Զ����á� �������������ɹ�����ִ�����ɺ��� boost::asio::async_write() ��ͨ�� socket ���ͱ����� data �е���Ϣ
void Server::accept_handler(session_ptr _chatSession, const boost::system::error_code& _error) {
	if (!_error && _chatSession) {
		try {
			_chatSession->start();
			start();
		}
		catch (...) {
			return;
		}
	}
}