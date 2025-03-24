#include "CliSocket.hpp"
#include "PrivmsgCommand.hpp"
#include "Server.hpp"
#include "Channel.hpp"


void	PrivmsgCommand::execute(const std::string &args, CliSocket *client)
{
	std::vector<std::string> vecArgs = splitArgs(args);
	std::string textmsg = "";
	std::string msg;

	if (vecArgs.size() > 1)
		textmsg = vecArgs[1];

	if (!vecArgs[0].empty())
	{
		if (vecArgs[0][0] == '#' || vecArgs[0][0] == '&')
		{
			std::map<std::string, Channel *> OniChanUwU = Server::getInstance().getOniChanUwU();
			std::map<std::string, Channel *>::const_iterator it = OniChanUwU.begin();
			for ( ; it != OniChanUwU.end(); ++it)
			{
				if (sameString(vecArgs[0], it->first))
				{
					if (client->isInList(it->second->getMembers()) )
					{
						msg = client->getSource() + " PRIVMSG " + vecArgs[0] + " :" + textmsg;
						std::set<int> tmpList = it->second->getMembers();
						Channel::removeFromList(tmpList, client->getFd());
						broadcast(tmpList, msg);
						break ;
					}
					else
						return sendRpl(client, ERR_CANOTSENDTOCHAN, client->getNick().c_str(), vecArgs[0].c_str());
				}
			}
			if (it == OniChanUwU.end())
				return sendRpl(client, ERR_NOSUCHCHANNEL, client->getNick().c_str(),vecArgs[0].c_str());
		}
		else
		{
			std::vector<CliSocket *> vecClients = Server::getInstance().getClients();
			std::vector<CliSocket *>::const_iterator it2 = vecClients.begin();
			for ( ; it2 !=vecClients.end(); ++it2)
			{
				if (vecArgs[0] == (*it2)->getNick())
				{
					msg = client->getSource() + " PRIVMSG " + vecArgs[0] + " " + textmsg;
					break ;
					sendMsg((*it2), msg);
				}
			}
			if (it2 == vecClients.end())
				return sendRpl(client, ERR_NOSUCHNICK, client->getNick().c_str(),vecArgs[0].c_str());
		}
	}
}

// /connect 127.0.0.1 6657 0000