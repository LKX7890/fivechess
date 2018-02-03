#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "server.hpp"

int main(int argc, char **argv)
{
	try {
		std::cout << "server start." << std::endl;
		// ����������  
		boost::asio::io_service ios;
		// ����ķ�������ַ��˿�  
		boost::asio::ip::tcp::endpoint endpotion(boost::asio::ip::tcp::v4(), 13695);
		// ����Serverʵ��  
		Server server(ios, endpotion);
		// �����첽�����¼�����ѭ��  
		server.run();
	}
	catch (std::exception& _e) {
		std::cout << _e.what() << std::endl;
	}
	std::cout << "server end." << std::endl;
	return 0;
}