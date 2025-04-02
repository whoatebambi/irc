#include "CommandPart.hpp"

void CommandPart::execute(const std::string &args, Client *client)
{
	// std::cout << INVERSE_BG << BLUE << "PART args: " BOLD << args << RESET << std::endl;
	std::vector<std::string> argVec = splitArgs(args);
	if (argVec.empty() || argVec.size() > 2)
		return (Reply::sendNumReply(client, ERR_NEEDMOREPARAMS, "PART"));
	
	std::vector<std::string> channelVec = splitString(argVec[0], ',');
	std::string partMsg;
	if (argVec.size() == 2)
		partMsg = argVec[1];
	if (!isValidPartMsg(partMsg))
		partMsg = ""; // drop silently

	const std::set<Channel*> &channelSet = Server::getInstance().get_channelSet();
	for (std::vector<std::string>::const_iterator it = channelVec.begin(); it != channelVec.end(); ++it)
	{
		const std::string &channelName = *it;
		Channel *channel = NULL;
		for (std::set<Channel*>::const_iterator itSet = channelSet.begin(); itSet != channelSet.end(); ++itSet)
		{
			if ((*itSet)->get_name() == channelName)
			{
				channel = *itSet;
				break;
			}
		}
		if (!channel)
		{
			Reply::sendNumReply(client, ERR_NOSUCHCHANNEL, channelName);
			continue;
		}
		if (!channel->isInChannel(client))
		{
			Reply::sendNumReply(client, ERR_NOTONCHANNEL, channelName);
			continue;
		}
		channel->partChannel(client, partMsg);
		if (channel->get_memberSet().size() == 0)
			Server::getInstance().removeChannel(channel);
	}
}

bool isValidPartMsg(const std::string &msg)
{
	if (msg.empty())
		return (true);

	for (size_t i = 0; i < msg.length(); ++i)
	{
		char c = msg[i];
		if (c < 32 || c > 126)
			return (false);
	}
	return (true);
}
