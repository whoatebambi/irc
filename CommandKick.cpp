#include "CommandKick.hpp"

static bool	canKick(Channel *channel, Client *client, Client *toKick, const std::string &channelName);
static void		performKick(Channel *channel, Client *client, Client *toKick, const std::string &kickMsg);

void CommandKick::execute(const std::string &args, Client *client)
{
	// std::cout << INVERSE_BG << BLUE << "KICK args: " BOLD << args << RESET << std::endl;
	std::vector<std::string> argVec = splitArgs(args);
	if (argVec.size() < 2 || argVec.size() > 3)
		return (Reply::sendNumReply(client, ERR_NEEDMOREPARAMS, "KICK"));

	std::vector<std::string> channelVec = splitString(argVec[0], ',');
	const std::string &nickname = argVec[1];
	std::string kickMsg = (argVec.size() == 3) ? argVec[2] : ""; //: client->get_nickname();

	Client *toKick = Server::getInstance().get_client(nickname);
	if (!toKick)
		return (Reply::sendNumReply(client, ERR_NOSUCHNICK, nickname));

	for (size_t i = 0; i < channelVec.size(); ++i)
	{
		std::string &channelName = channelVec[i];
		Channel *channel = Channel::findChannel(channelName);
		if (!canKick(channel, client, toKick, channelName))
			continue;
		performKick(channel, client, toKick, kickMsg);
	}
}

bool	canKick(Channel *channel, Client *client, Client *toKick, const std::string &channelName)
{
	if (!channel)
		return (Reply::sendNumReply(client, ERR_NOSUCHCHANNEL, channelName), false);
	if (!channel->isInChannel(client))
		return (Reply::sendNumReply(client, ERR_NOTONCHANNEL, channelName), false);
	if (!channel->isOperator(client))
		return (Reply::sendNumReply(client, ERR_CHANOPRIVSNEEDED, channelName), false);
	if (!channel->isInChannel(toKick))
		return (Reply::sendNumReply(client, ERR_USERNOTINCHANNEL, toKick->get_nickname(), channelName), false);
	return (true);
}

void performKick(Channel *channel, Client *client, Client *toKick, const std::string &kickMsg)
{
	std::string msg = "KICK " + channel->get_name() + " " + toKick->get_nickname();
	if (!kickMsg.empty())
		msg += " :" + kickMsg;
	Reply::sendBroadcast(channel->generateMembersFd(), client, msg);
	Server::getInstance().removeClient(toKick->get_fd());
	// channel->removeMember(toKick);
	// channel->removeInvite(toKick);
	// channel->removeOperator(toKick);
}
