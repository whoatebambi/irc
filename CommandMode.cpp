#include "CommandMode.hpp"

void CommandMode::execute(const std::string &args, Client *client)
{
	std::cout << INVERSE_BG << BLUE << "MODE args: " BOLD << args << RESET << std::endl;

	std::vector<std::string> arg = CommandMode::splitArgs(args);
	if (arg.size() < 1)
		return (Reply::sendNumReply(client, ERR_NEEDMOREPARAMS, "MODE"));

	std::string target = arg[0];
	if (target == client->getNickname())
	{
		if (arg.size() == 1)
			return (Reply::sendNumReply(client, RPL_UMODEIS, "+iiiiii"));
		return (Reply::sendNumReply(client, ERR_USERSDONTMATCH, "OR FOR SELF")); // custom because we don't handle user mode
	}

	refactorTarget(target); // or remove # ?
	Channel *channel = Channel::findChannel(target);
	if (!channel)
		return (Reply::sendNumReply(client, ERR_NOSUCHCHANNEL, target));
	if (!channel->isUserInChannel(client))
		return (Reply::sendNumReply(client, ERR_NOTONCHANNEL, target));
	
	if (arg.size() == 1)
		return (sendChannelMode(channel, client));

	if(!channel->isOperator(client))
		return (Reply::sendNumReply(client, ERR_CHANOPRIVSNEEDED, target));

	applyModes(channel, client, arg);
}

void CommandMode::refactorTarget(std::string &target)
{
	if (!target.empty() && target[0] != '#')
		target = "#" + target;
}

void CommandMode::sendChannelMode(Channel *chan, Client *client)
{
	std::string modes = "+";
	std::string params;

	if (chan->isInviteOnly())
		modes += "i";
	if (chan->isTopicLocked())
		modes += "t";
	if (!chan->getKey().empty()) {
		modes += "k";
		params += " " + chan->getKey();
	}
	if (chan->getUserLimit() > 0) {
		modes += "l";
		std::stringstream ss;
		ss << chan->getUserLimit();
		params += " " + ss.str();
	}
	Reply::sendNumReply(client, RPL_CHANNELMODEIS, chan->getName(), modes + params);
}

void CommandMode::applyModes(Channel* channel, Client *client, const std::vector<std::string>& args)
{
	std::string modeStr = args[1];
	bool adding = true;
	size_t j = 2;
	std::string modes;
	std::string params;
	for (size_t i = 0; i < modeStr.length(); ++i)
	{
		char c = modeStr[i];
		if (c == '+') adding = true;
		else if (c == '-') adding = false;
		else if (c == 'i') handleMode_i(channel, adding, modes);
		else if (c == 't') handleMode_t(channel, adding, modes);
		else if (c == 'k') handleMode_k(channel, client, args, adding, j, modes, params);
		else if (c == 'l') handleMode_l(channel, args, adding, j, modes, params);
		else if (c == 'o') handleMode_o(channel, client, args, adding, j, modes, params);
		else
		{
			std::string unknown(1, c);
			Reply::sendNumReply(client, ERR_UNKNOWNMODE, unknown);
		}
	}
	if (!modes.empty())
		Reply::sendBroadcast(channel->get_clientsSet(), client, "MODE " + channel->getName() + " " + modes + params);
}

void CommandMode::handleMode_i(Channel* channel, bool adding, std::string& modes)
{
	channel->setInviteOnly(adding);
	if (adding)
		modes += "+i";
	else
		modes += "-i";
}

void CommandMode::handleMode_t(Channel* channel, bool adding, std::string& modes)
{
	channel->setTopicLocked(adding);
	if (adding)
		modes += "+t";
	else
		modes += "-t";
}

void CommandMode::handleMode_k(Channel* channel, Client* client, const std::vector<std::string>& args, bool adding, size_t& j, std::string& modes, std::string& params)
{
	if (adding)
	{
		if (j >= args.size())
			return;

		if (!channel->getKey().empty()) {
			Reply::sendNumReply(client, ERR_KEYSET, channel->getName());
			return;
		}
		channel->setKey(args[j]);
		modes += "+k";
		params += " " + args[j++];
	}
	else
	{
		channel->setKey("");
		modes += "-k";
	}
}

void CommandMode::handleMode_l(Channel* channel, const std::vector<std::string>& args, bool adding, size_t& j, std::string& modes, std::string& params)
{
	if (adding)
	{
		if (j >= args.size())
			return;

		std::stringstream ss(args[j]);
		size_t limit;
		ss >> limit;
		channel->setUserLimit(limit);
		modes += "+l";
		params += " " + args[j++];
	}
	else
	{
		channel->setUserLimit(0);
		modes += "-l";
	}
}

void CommandMode::handleMode_o(Channel* channel, Client* client, const std::vector<std::string>& args, bool adding, size_t& j, std::string& modes, std::string& params)
{
	if (j >= args.size())
		return;

	std::string nick = args[j++];
	Client* targetUser = Server::getInstance().findClient(nick);
	if (!targetUser || !channel->isUserInChannel(targetUser))
	{
		Reply::sendNumReply(client, ERR_NOSUCHNICK, nick);
		return;
	}
	if (adding)
		channel->addOperator(targetUser);
	else
		channel->removeOperator(targetUser);
	if (adding)
		modes += "+o";
	else
		modes += "-o";
	params += " " + nick;
}

std::vector<std::string> CommandMode::splitArgs(const std::string &input)
{
	std::vector<std::string> vec;
	std::istringstream iss(input);
	std::string word;

	while(iss >> word)
		vec.push_back(word);
	return vec;
}

// /NAMES to show @ ops.
// Automatic op assignment when the last op leaves or disconnects (to prevent a “dead” channel).