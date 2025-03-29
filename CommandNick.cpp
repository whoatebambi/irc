#include "CommandNick.hpp"

void CommandNick::execute(const std::string &args, Client *client)
{
	// std::cout << INVERSE_BG << BLUE << "NICK args: " BOLD << args << RESET << std::endl;
	if (args.empty())
		return (Reply::sendNumReply(client, ERR_NONICKNAMEGIVEN));

	if (!isValidNick(args))
		return (Reply::sendNumReply(client, ERR_ERRONEUSNICKNAME, args));

	if (Server::getInstance().isClient(args)) // && client->get_nickname() != args
		return Reply::sendNumReply(client, ERR_NICKNAMEINUSE, args);

	std::string oldMask = client->get_mask();
	client->set_nickname(args);
	client->set_mask();

	const std::set<Channel*> &channelSet = Server::getInstance().getChannelSet();
	std::set<int> broadcastFds;
	for (std::set<Channel*>::const_iterator it = channelSet.begin(); it != channelSet.end(); ++it)
	{
		Channel *channel = *it;
		if (channel->isInChannel(client))
		{
			const std::set<int> &fds = channel->generateMembersFd();
			broadcastFds.insert(fds.begin(), fds.end()); // merge into set (no duplicates)
		}
	}
	// if (it == channelSet.end())
		// need to handle this case? 
	std::string msg = oldMask + " " + "NICK :" + client->get_nickname() + "\r\n";
	if (broadcastFds.empty())
	{
		send(client->get_fd(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
		return;
	}
	// Reply::sendBroadcast(broadcastFds, client, msg);
	for (std::set<int>::const_iterator it = broadcastFds.begin(); it != broadcastFds.end(); ++it)
	{
		std::cout << INVERSE_BG << RED << ">>> " << BOLD << "server->FD<" << *it << "> " << msg << RESET ;
		send(*it, msg.c_str(), msg.size(), MSG_NOSIGNAL);
	}
}

bool isValidNick(const std::string &nick)
{
	const std::string allowed = "[]\\^{}_|`";

	if (nick.empty() || nick.length() > 9)
		return (false);

	char first = nick[0];
	if (isdigit(first) || first == '-')
		return (false);

	for (size_t i = 0; i < nick.length(); ++i)
	{
		char c = nick[i];
		if (isalpha(c) || isdigit(c) || allowed.find(c) != std::string::npos)
			continue;
		return (false);
	}
	return (true);
}
