#include "CommandNick.hpp"

void CommandNick::execute(const std::string &args, Client *client)
{
	std::cout << "NICK command executed with args: " << args << std::endl;
	
	std::string arr = "[]{}\\|";

	std::string oldnick = client->getNickname();
	const std::string oldmask = client->getSource();
	std::cout << "Old mask: " << oldmask << std::endl;
    client->setNickname(args);
    const std::string newmask = client->getSource();
	std::cout << "New mask: " << newmask << std::endl;

    // Server::getInstance().updateList(oldmask, newmask);

    // std::set<int> pairs = Server::getInstance().getSharedChans(client);
    // broadcast(pairs, ":" + oldnick + " NICK " + args);
}
