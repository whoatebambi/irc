#include "CommandUser.hpp"

void CommandUser::execute(const std::string &args, Client *client)
{
	// std::cout << INVERSE_BG << BLUE << "USER args: " BOLD << args << RESET << std::endl;

	std::vector<std::string> arg = splitArgs(args);
	if (arg.size() < 4)
		return (Reply::sendNumReply(client, ERR_NEEDMOREPARAMS, "USER"));
	if (!(client->getUsername().empty()))
		return (Reply::sendNumReply(client, ERR_ALREADYREGISTRED));

	client->setUsername(arg[0]);
	client->setRealname(arg[3]);
}
