#include "CommandPart.hpp"

void CommandPart::execute(const std::string &args, Client *client)
{
	// std::cout << INVERSE_BG << BLUE << "PART args: " BOLD << args << RESET << std::endl;
	if (args.empty()) // already handled by Irssi but safe to add
		return (Reply::sendNumReply(client, ERR_NEEDMOREPARAMS, "PART"));

	std::vector<std::string> argsVector = splitArgs(args);
	std::vector<std::string> channelVec = splitString(argsVector[0], ',');
	std::string partMsg;
	if (argsVector.size() >= 2)
	{
		partMsg = argsVector[1];
		if (!isValidPartMsg(partMsg))
			partMsg = ""; // drops silently
	}

	const std::map<std::string, Channel*> &channelMap = Server::getInstance().getChannelMap();
	for (std::vector<std::string>::const_iterator it = channelVec.begin(); it != channelVec.end(); ++it)
	{
		const std::string &channelName = *it;
		std::map<std::string, Channel*>::const_iterator found = channelMap.find(channelName);
		if (found == channelMap.end())
		{
			Reply::sendNumReply(client, ERR_NOSUCHCHANNEL, channelName);
			continue;
		}
		Channel *channel = found->second;
		if (!channel->isInChannel(client))
		{
			Reply::sendNumReply(client, ERR_NOTONCHANNEL, channelName);
			continue;
		}
		channel->partChannel(client, partMsg); // This should do the broadcasting + removal
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
