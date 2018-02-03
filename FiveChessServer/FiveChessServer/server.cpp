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

	// 异步接收连接请求
	acceptor_.async_accept(new_chat_session->socket(),
		boost::bind(&Server::accept_handler, this, new_chat_session,
		boost::asio::placeholders::error));
}

void Server::run(void) {
	// 该方法调用完之后，回调对象才会被调用
	ioService_.run();
}

void Server::callback_session(std::string /*_fromIp*/, std::string /*_info*/) {
	return;
}

// 当一个PC试图建立一个连接时，accept_handler() 被自动调用。 如果该连接请求成功，就执行自由函数 boost::asio::async_write() 来通过 socket 发送保存在 data 中的信息
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