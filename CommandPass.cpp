#include "CommandPass.hpp"

void CommandPass::execute(const std::string &args, Client *client)
{
	// std::cout << INVERSE_BG << BLUE << "PASS args: " BOLD << args << RESET << std::endl;

	if (args.empty())
		return (Reply::sendNumReply(client, ERR_NEEDMOREPARAMS, "PASS"));
	if (args == (Server::getInstance().get_pass()))
	{
		if (!client->get_isAuth())
			client->set_isAuth();
		else
			return (Reply::sendNumReply(client, ERR_ALREADYREGISTRED));
	}
	//ajouter si client pas auth client.setauth(true) sinon err client already auth
	else
	{
		Server::getInstance().removeClient(client->get_fd());
		std::cout << "Client <" << client->get_fd() << "> removed" << std::endl;
	}
}
