//a simple tcp synchronous server side chat application
#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include <string_view>
#include <algorithm>
using namespace std::literals::string_literals;
unsigned int PORT_NUMBER = 8001;
constexpr unsigned int MAX_MSG_LENGTH = 1024;
std::size_t read_complete(std::string_view msg, boost::system::error_code const& err, std::size_t bytes) {
	if (err) return 0;
	bool found = std::find(std::begin(msg), std::end(msg), '\n') < std::begin(msg) + bytes;
	return found ? 0 : 1;
}
int main() {
	boost::asio::io_service service;
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), PORT_NUMBER);
	boost::asio::ip::tcp::acceptor acceptor(service, endpoint);
	std::cout << "connecting...\n";
	boost::system::error_code err;
	boost::asio::ip::tcp::socket sock(service);
	acceptor.accept(sock, err);
	if (err) {
		std::cout << "ERROR: " << err.message() << "\n";
	}
	else {
		std::cout << "you are connected and chatting as server...";
		char incoming_message_buffer[MAX_MSG_LENGTH];
		std::string outgoing_message_buffer;
		while (sock.is_open()) {
			std::cout << "\n\nClient: ";
			auto bytes = boost::asio::read(sock, boost::asio::buffer(incoming_message_buffer), std::bind(read_complete,
				incoming_message_buffer, std::placeholders::_1, std::placeholders::_2));
			std::string msg_received(incoming_message_buffer, incoming_message_buffer + bytes);
			std::cout << msg_received;
			std::cout << "\n\n                          Server: ";
			getline(std::cin, outgoing_message_buffer);
			outgoing_message_buffer += "\n";
			sock.write_some(boost::asio::buffer(outgoing_message_buffer));
			std::cin.clear();
		}
	}
}