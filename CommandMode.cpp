#include "CommandMode.hpp"

void CommandMode::execute(const std::string &args, Client *client)
{
	// std::cout << INVERSE_BG << BLUE << "MODE args: " BOLD << args << RESET << std::endl;
	if (args.empty())
		return (Reply::sendNumReply(client, ERR_NEEDMOREPARAMS, "MODE"));

	std::vector<std::string> argVector = splitArgs(args);
	const std::string &target = argVector[0];

	if (target[0] == '#') // handle channel modes
	{
		Channel *channel = Channel::findChannel(target);

		if (!channel)
			return (Reply::sendNumReply(client, ERR_NOSUCHCHANNEL, target));
		if (!channel->isInChannel(client))
			return (Reply::sendNumReply(client, ERR_NOTONCHANNEL, target));
		if (argVector.size() == 2 && (argVector[1] == "b" || argVector[1] == "+b")) // special request from IRSSI on JOIN
			return (Reply::sendNumReply(client, RPL_ENDOFBANLIST, target));
		if (argVector.size() == 1)
			return (sendModes(channel, client));

		if(!channel->isOperator(client))
			return (Reply::sendNumReply(client, ERR_CHANOPRIVSNEEDED, target));
		applyModes(channel, client, argVector);
	}
	else // handle user modes
	{
		if (target != client->get_nickname())
			return (Reply::sendNumReply(client, ERR_NOSUCHNICK, target));
		if (argVector.size() == 1 || (argVector.size() == 2 && argVector[1] == "+i"))
			return (Reply::sendNumReply(client, RPL_UMODEIS, "+i"));
		return (Reply::sendNumReply(client, ERR_USERSDONTMATCH, "Can't change mode for self.")); // custom: we don't handle user mode
	}	
}

void CommandMode::sendModes(Channel *chan, Client *client)
{
	std::string modes = "+";
	std::string params;

	if (chan->get_inviteOnly())
		modes += "i";
	if (chan->get_topicLocked())
		modes += "t";
	if (!chan->get_key().empty()) {
		modes += "k";
		params += " " + chan->get_key();
	}
	if (chan->get_userLimit() > 0) {
		modes += "l";
		std::stringstream ss;
		ss << chan->get_userLimit();
		params += " " + ss.str();
	}
	Reply::sendNumReply(client, RPL_CHANNELMODEIS, chan->get_name(), modes + params);
}

void CommandMode::applyModes(Channel* channel, Client *client, const std::vector<std::string>& args)
{
    CommandContext ctx(client, channel, args);

	for (size_t i = 0; i < ctx.modeStr.length(); ++i)
	{
		char c = ctx.modeStr[i];
		if (c == '+') ctx.adding = true;
		else if (c == '-') ctx.adding = false;
		else if (c == 'i') handleMode_i(ctx);
		else if (c == 't') handleMode_t(ctx);
		else if (c == 'k') handleMode_k(ctx);
		else if (c == 'l') handleMode_l(ctx);
		else if (c == 'o') handleMode_o(ctx);
		else
			Reply::sendNumReply(client, ERR_UNKNOWNMODE, std::string(1, c));
	}
	if (!ctx.modes.empty())
		Reply::sendBroadcast(channel->generateMembersFd(), client, "MODE " + channel->get_name() + " " + ctx.modes + ctx.params);
}

void CommandMode::handleMode_i(CommandContext &ctx)
{
	if (ctx.adding)
	{
		ctx.channel->set_inviteOnly(true);
		ctx.modes += "+i";
		std::set<Client*> const	&memberSet = ctx.channel->get_memberSet();
		for (std::set<Client*>::const_iterator it = memberSet.begin(); it != memberSet.end(); ++it)
			ctx.channel->addInvite(*it);
	}
	else
	{
		ctx.modes += "-i";
		ctx.channel->set_inviteOnly(false);
		std::set<Client*> const	&memberSet = ctx.channel->get_memberSet();
		for (std::set<Client*>::const_iterator it = memberSet.begin(); it != memberSet.end(); ++it)
			ctx.channel->removeInvite(*it);
	}
}

void CommandMode::handleMode_t(CommandContext &ctx)
{
	ctx.channel->setTopicLocked(ctx.adding);
	if (ctx.adding)
		ctx.modes += "+t";
	else
		ctx.modes += "-t";
}

void CommandMode::handleMode_k(CommandContext &ctx)
{
	if (ctx.adding)
	{
		if (ctx.index >= ctx.args.size())
			return;

		if (!ctx.channel->get_key().empty())
			return (Reply::sendNumReply(ctx.client, ERR_KEYSET, ctx.channel->get_name()));	

		ctx.channel->set_key(ctx.args[ctx.index]);
		ctx.modes += "+k";
		ctx.params += " " + ctx.args[ctx.index++];
	}
	else
	{
		ctx.channel->set_key("");
		ctx.modes += "-k";
	}
}

void CommandMode::handleMode_l(CommandContext &ctx)
{
	if (ctx.adding)
	{
		if (ctx.index >= ctx.args.size())
			return;

		const std::string& arg = ctx.args[ctx.index];
		if (arg.empty() || arg.find_first_not_of("0123456789") != std::string::npos)
			return (Reply::sendNumReply(ctx.client, ERR_UNKNOWNMODE, "l"));

		int limit = strtol(arg.c_str(), NULL, 10);
		if (limit <= 0)
			return (Reply::sendNumReply(ctx.client, ERR_UNKNOWNMODE, "l"));
		
		ctx.channel->set_userLimit(limit);
		ctx.modes += "+l";
		ctx.params += " " + ctx.args[ctx.index++];
	}
	else
	{
		ctx.channel->set_userLimit(0);
		ctx.modes += "-l";
	}
}

void CommandMode::handleMode_o(CommandContext &ctx)
{
	if (ctx.index >= ctx.args.size())
		return;

	std::string nick = ctx.args[ctx.index++];
	Client* targetUser = Server::getInstance().get_client(nick);
	if (!targetUser || !ctx.channel->isInChannel(targetUser))
		return (Reply::sendNumReply(ctx.client, ERR_NOSUCHNICK, nick));

	if (ctx.adding)
	{
		ctx.channel->addOperator(targetUser);
		ctx.modes += "+o";
	}
	else
	{
		ctx.channel->removeOperator(targetUser);
		ctx.modes += "-o";
	}
	ctx.params += " " + nick;
}
