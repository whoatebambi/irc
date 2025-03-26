#include "CommandPing.hpp"

void CommandPing::execute(const std::string &args, Client *client)
{
	// std::cout << INVERSE_BG << BLUE << "PING args: " BOLD << args << RESET << std::endl;
	
	Reply::sendReply(client, "PONG :" + args);
}
