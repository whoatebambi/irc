#include "CapCommand.hpp"

void CapCommand::execute(const std::string &args, Client *client)
{
	std::cout << "CAP command executed with args: " << args << std::endl;
	(void)client;
	
	// if (args == "LS")
    //     sendMsg(client, "CAP * LS :");
    // else if (args == "END")
    //     sendWelcome(client);
    // if (args == "LS")
    // {
	// 	std::string msg = "CAP * LS :PROUD_TO_BE_A_FURRY";
	// 	msg += "\r\n";
    //     send(client->getFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
    // }
    // else if (args == "END")
	// {
	// 	std::cout << "CAP negotiation ended" << std::endl;
    //     std::string msg = "001 " + client->getNickname() + " :Welcome to the IRC server";
    //     msg += "\r\n";
    //     send(client->getFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
	// }
}
