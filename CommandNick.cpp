#include "CommandNick.hpp"

void CommandNick::execute(const std::string &args, Client *client)
{
	std::cout << "NICK command executed with args: " << args << std::endl;
	
	std::string arr = "[]{}\\|";

	std::string oldnick = client->getNickname();



	// std::string msg = "<<<< NICKKKKKKKK ";
	// msg += "\r\n";
	// send(client->getFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
	//client->setNickname(arg[1]);
}
