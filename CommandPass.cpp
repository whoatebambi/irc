#include "CommandPass.hpp"

void CommandPass::execute(const std::string &args, Client *client)
{
	// std::cout << INVERSE_BG << BLUE << "PASS args: " BOLD << args << RESET << std::endl;
	if (args.empty())
		return (Reply::sendNumReply(client, ERR_NEEDMOREPARAMS, "PASS"));
	if (client->get_isAuth())
		return (Reply::sendNumReply(client, ERR_ALREADYREGISTRED));
	if (args != (Server::getInstance().get_password()))
	{
		client->set_isDead();
		std::cout << "Client <" << client->get_fd() << "> flagged as DEAD during PASS" << std::endl;
		return;
	}
	else
		client->set_isAuth();
}
