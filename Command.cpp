#include "Command.hpp"

void Command::sendMsg(Client *client, std::string msg)
{
    std::cout << "SERVER To FD: " << client->getFd() << " >> " << msg << std::endl;
    msg += "\r\n";
    send(client->getFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
}
