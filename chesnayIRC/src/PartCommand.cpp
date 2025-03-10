#include "PartCommand.hpp"
#include "Server.hpp"
#include "CliSocket.hpp"
#include "Channel.hpp"


std::vector<Channel *> PartCommand::chanExist(std::vector<std::string> &chans, CliSocket *client)
{
    std::vector<Channel*> vecChan;
    std::map<std::string, Channel*> OniChanUwU = Server::getInstance().getOniChanUwU();

    for (std::vector<std::string>::iterator it = chans.begin(); it != chans.end(); ++it)
    {
        bool found = false;
        for (std::map<std::string, Channel*>::iterator it2 = OniChanUwU.begin(); it2 != OniChanUwU.end(); ++it2)
        {
            if (sameString(*it, it2->first))
            {
                vecChan.push_back(it2->second);
                found = true;
                break;
            } 
        }
        if (!found)
            sendRpl(client, ERR_NOSUCHCHANNEL, client->getNick().c_str(), it->c_str());
    }
    return vecChan;
}



void	notifyPart(std::vector<std::string> vecArgs, Channel *chan, CliSocket *client)
{
	std::string msg;

	msg = client->getSource() + " PART " + chan->getName();
	if (vecArgs.size() > 1)
	{
		msg = msg + " :" + vecArgs[1];
	}
	Command::broadcast(chan->getMembers(), msg);
}

void PartCommand::execute(const std::string &args, CliSocket *client)
{
	std::vector<std::string> vecArgs = splitArgs(args);

	if (vecArgs.size() >= 1 && vecArgs.size() <= 2)
	{
		std::vector<std::string> chans = splitString(vecArgs[0], ',');
		std::vector<Channel *> vecChan = chanExist(chans, client);
		for (std::vector<Channel *>::iterator it = vecChan.begin(); it != vecChan.end(); ++it) 
		{
			if(client->isInList((*it)->getMembers()))
			{
				notifyPart(vecArgs, *it, client);
				Channel::removeFromList((*it)->getMembers(), client->getFd());
			}
			else
				sendRpl(client, ERR_NOTONCHANNEL, client->getNick().c_str(), (*it)->getName().c_str());
		}
	}
	else
		sendRpl(client, ERR_NEEDMOREPARAMS, client->getNick().c_str(), " :Invalid amount of parameters");
}
