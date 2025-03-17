#include "CommandPing.hpp"

void CommandPing::execute(const std::string &args, Client *client)
{
	std::cout << INVERSE_BG << BLUE << "PING command executed with args: " BOLD << args << RESET << std::endl;
	
	std::string msg = ":" + args;
	sendMsg(client, msg);
}
