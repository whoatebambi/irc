#include "CommandInvite.hpp"

void CommandInvite::execute(const std::string &args, Client *client)
{
	// std::cout << INVERSE_BG << BLUE << "INVITE args: " BOLD << args << RESET << std::endl;
	if (args.empty())
		return (Reply::sendNumReply(client, ERR_NEEDMOREPARAMS, "INVITE"));
	
	std::vector<std::string> argVector = splitArgs(args);
	if (argVector.size() != 2)
		return (Reply::sendNumReply(client, ERR_NEEDMOREPARAMS, "INVITE"));

	const std::string &nickname = argVector[0];
	const std::string &channelName = argVector[1];

	Channel *channel = Channel::findChannel(channelName);
	if (!channel)
		return (Reply::sendNumReply(client, ERR_NOSUCHCHANNEL, channelName));
	if (!channel->isInChannel(client))
		return (Reply::sendNumReply(client, ERR_NOTONCHANNEL, channelName));
	if(!channel->isOperator(client))
		return (Reply::sendNumReply(client, ERR_CHANOPRIVSNEEDED, channelName));

	Client *invitee = Server::getInstance().get_client(nickname);
	if (!invitee || !invitee->get_isAuth())
		return (Reply::sendNumReply(client, ERR_NOSUCHNICK, nickname));
	if (channel->isInChannel(invitee))
		return (Reply::sendNumReply(client, ERR_USERONCHANNEL, nickname, channelName));

	channel->addInvite(invitee);
	std::string msg = client->get_mask() + " INVITE " + nickname + " :" + channelName;
	Reply::sendReply(client, msg); // :nick!user@host INVITE invitee :#channel
	Reply::sendNumReply(invitee, RPL_INVITING, channelName, nickname);
	Reply::sendNumReply(client, RPL_INVITING, channelName, nickname); // optional
}
