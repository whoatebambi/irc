#include "CommandMode.hpp"

void CommandMode::execute(const std::string &args, Client *client)
{
	std::cout << INVERSE_BG << BLUE << "MODE command executed with args: " BOLD << args << RESET << std::endl;

	std::vector<std::string> arg = CommandMode::splitArgs(args);
	if (arg.size() < 1)
		return (NumericReplies::sendNumReply(client, ERR_NEEDMOREPARAMS, "MODE"));

	std::string target = arg[0];
	if (target == client->getNickname() && (arg.size() == 1 || arg[1] == "+i"))
		return (NumericReplies::sendNumReply(client, RPL_UMODEIS, "+iiiiii"));

	refactorTarget(target);
	// std::cout << BLUE << "target = " << target << RESET << std::endl;
	Channel *channel = Channel::findChannel(target);
	if (!channel)
		return (NumericReplies::sendNumReply(client, ERR_NOSUCHCHANNEL, target));

	if (arg.size() == 1)
		return (NumericReplies::sendNumReply(client, RPL_CHANNELMODEIS, target, "+nrt"));
		// std::string getChannelMode(Client *client, std::string target)

	if (client->getSource() != channel->getFounderMask())// && !client->isInList(chan->getOpsList()))
	{
		std::cout << BLUE << client->getSource() << " != " << channel->getFounderMask() << RESET << std::endl;
		return (NumericReplies::sendNumReply(client, ERR_CHANOPRIVSNEEDED, target));
	}
	
}

void CommandMode::refactorTarget(std::string &target)
{
	if (!target.empty() && target[0] != '#')
		target = "#" + target;
}
