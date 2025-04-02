#include "CommandPrivMsg.hpp"

static void	handleChannelMsg(const std::string &target, const std::string &msgArg, Client *client);
static void	handlePrivateMsg(const std::string &target, const std::string &msgArg, Client *client);

void CommandPrivMsg::execute(const std::string &args, Client *client)
{
	// std::cout << INVERSE_BG << BLUE << "PRIVMSG args: " BOLD << args << RESET << std::endl;
	if (args.empty())
		return (Reply::sendNumReply(client, ERR_NEEDMOREPARAMS, "PRIVMSG"));

	std::vector<std::string> argsVec = splitArgs(args);
	if (argsVec.empty() || argsVec[0].empty())
		return (Reply::sendNumReply(client, ERR_NORECIPIENT, "PRIVMSG"));

	std::string target = argsVec[0];
	std::string msgArg = argsVec.size() > 1 ? argsVec[1] : "";

	if (msgArg.empty())
		return (Reply::sendNumReply(client, ERR_NOTEXTTOSEND, ""));

	if (target[0] == '#')
		handleChannelMsg(target, msgArg, client);
	else
		handlePrivateMsg(target, msgArg, client);
}

void	handleChannelMsg(const std::string &target, const std::string &msgArg, Client *client)
{
	const std::set<Channel*> &channelSet = Server::getInstance().get_channelSet();
	Channel *channel = NULL;

	for (std::set<Channel*>::const_iterator it = channelSet.begin(); it != channelSet.end(); ++it)
	{
		if (sameString((*it)->get_name(), target))
		{
			channel = *it;
			break;
		}
	}

	if (!channel)
		return (Reply::sendNumReply(client, ERR_NOSUCHCHANNEL, target));

	if (!channel->isInChannel(client))
		return (Reply::sendNumReply(client, ERR_CANNOTSENDTOCHAN, target));

	std::set<int> fdSet = channel->generateMembersFd();
	fdSet.erase(client->get_fd());

	Reply::sendBroadcast(fdSet, client, "PRIVMSG " + target + " :" + msgArg);
}

void	handlePrivateMsg(const std::string &target, const std::string &msgArg, Client *client)
{
	const std::vector<Client*> &clients = Server::getInstance().get_clientVec();
	Client *receiver = NULL;

	for (std::vector<Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if ((*it)->get_nickname() == target)
		{
			receiver = *it;
			break;
		}
	}

	if (!receiver)
		return (Reply::sendNumReply(client, ERR_NOSUCHNICK, target));

	std::string msgFull = client->get_mask() + " PRIVMSG " + target + " :" + msgArg + "\r\n";
	std::cout << INVERSE_BG << RED << ">>> " << BOLD << " to fd<" << receiver->get_fd() << "> " << msgFull << RESET ;
	send(receiver->get_fd(), msgFull.c_str(), msgFull.size(), MSG_NOSIGNAL);
}
