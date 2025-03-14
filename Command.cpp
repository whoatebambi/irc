#include "Command.hpp"
#include "Client.hpp"

void Command::sendMsg(Client *client, std::string msg)
{
	// std::cout << "msg = " << msg << std::endl;
	msg += "\r\n";
	send(client->getFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
}

void Command::sendRpl(Client *client, int err, std::string msg)
{
	std::stringstream ss;
    ss << err << ": " << msg;
    std::string msg2 = ss.str();
	// std::cout << "msg2 = " << msg2 << std::endl;
    sendMsg(client, msg2);
}
