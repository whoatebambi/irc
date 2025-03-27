#include "CommandPrivMsg.hpp"

void CommandPrivMsg::execute(const std::string &args, Client *client)
{
	std::vector<std::string> argsVec = splitArgs(args);
	std::string msgArg = "";

	if (argsVec.size() > 1)
		msgArg = argsVec[1];
	if (!argsVec[0].empty())
	{
		if (argsVec[0][0] == '#' || argsVec[0][0] == '&')
		{
			std::map<std::string, Channel *> channel = Server::getInstance().getChannelMap();
			std::map<std::string, Channel *>::const_iterator it = channel.begin();
			for ( ; it != channel.end(); ++it)
			{
				if (sameString(argsVec[0], it->first))
				{
					if (client->isInList(it->second->getMembers()))
					{
						std::set<int> tmplist = it->second->get_membersFd();
						Channel::removeFromList(tmplist, client->getFd());
						Reply::sendBroadcast(tmplist, client, "PRIVMSG " + argsVec[0] + " :" + msgArg);  // check ":"
						break ;
					}
					else
						return (Reply::sendNumReply(client, ERR_CANNOTSENDTOCHAN, argsVec[0]));
				}
			}
			if (it == channel.end())
				return (Reply::sendNumReply(client, ERR_NOSUCHCHANNEL, argsVec[0]));
		}
		else
		{
			std::vector<Client *> clientsVec = Server::getInstance().getClients();
			std::vector<Client *>::const_iterator it2 = clientsVec.begin();
			for ( ; it2 != clientsVec.end(); ++it2)
			{
				if (argsVec[0] == (*it2)->get_nickname())
				{
					Reply::sendNumReply((*it2), RPL_AWAY, "PRIVMSG", argsVec[0] + " " + msgArg);
					break ;
				}
			}
			if (it2 == clientsVec.end())
				return (Reply::sendNumReply(client, ERR_NOSUCHNICK, client->get_nickname()));
		}
	}
	return;
}
