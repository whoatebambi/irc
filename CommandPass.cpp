#include "CommandPass.hpp"

void CommandPass::execute(const std::string &args, Client *client)
{
	std::cout << INVERSE_BG << BLUE << "PASS command executed with args: " BOLD << args << RESET << std::endl;

	if (args == (Server::getInstance().getPass())) {

		std::cout << GREEN << BOLD << "good password,on est content" << RESET << std::endl;
		if (!client->getIsAuth())
			client->setIsAuth();
		else
			std::cout << RED << BOLD << "err: client already auth" << RESET << std::endl;
	}

		//ajouter si client pas auth client.setauth(true) sinon err client already auth
	else {
		
		std::cout << RED << BOLD << "mauvais password, c'est mal :/" << RESET << std::endl;		
		Server::getInstance().RemoveClient(client->getFd());
		std::cout << RED << BOLD << "client <" << client->getFd() << "> removed" << RESET << std::endl;
	}
}
