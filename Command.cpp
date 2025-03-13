#include "Command.hpp"
#include "Client.hpp"

void Command::sendMsg(Client *client, std::string msg)
{
	msg += "\r\n";
	send(client->getFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
}
