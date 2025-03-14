#include "CommandNick.hpp"

void CommandNick::execute(const std::string &args, Client *client)
{
	std::cout << INVERSE_BG << BLUE << "NICK command executed with args: " BOLD << args << RESET << std::endl;
	
	std::string arr = "[]{}\\|";

	if (args.empty())
		return (sendMsg(client, ":431 :No nickname given")); // ERR_NONICKNAMEGIVEN
	for (size_t i = 0; i < args.length(); ++i)
	{
		if (!isalnum(args[i]) && (arr.find(args[i]) == std::string::npos))
			return (sendMsg(client, ":432 " + args + " :Erroneus nickname")); // ERR_ERRONEUSNICKNAME
	}
	// to do: ERR_NICKNAMEINUSE

	std::string oldnick = client->getNickname();
	const std::string oldmask = client->getSource();
	std::cout << "Old mask: " << oldmask << std::endl;
    client->setNickname(args);
    const std::string newmask = client->getSource();
	std::cout << "New mask: " << newmask << std::endl;

    // manage server->channelMap and add/remove mask from white list, ban list, ops list

    // find all fd of other clients that share a channel with this client

    // broadcast nickname inside relevant channels
}
