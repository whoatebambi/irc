#include "CommandQuit.hpp"

void CommandQuit::execute(const std::string &args, Client *client)
{
	// std::cout << INVERSE_BG << BLUE << "QUIT args: " BOLD << args << RESET << std::endl;
	std::vector<std::string> argVec = splitArgs(args);
	// if (argVec.size() > 1) // already handled by irssi
	// 	return (Reply::sendNumReply(client, ERR_NEEDMOREPARAMS, "PART"));
	std::string partMsg = "Quit";
	if (argVec.size() == 1 && isValidPartMsg(argVec[0]))
		partMsg = argVec[0];
	
	std::set<int> sharedMembersFd = Server::getInstance().getSharedMembersFd(client);
	std::string msg = "QUIT :" + partMsg;
	Reply::sendBroadcast(sharedMembersFd, client, msg);
}
