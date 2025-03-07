#include "KickCommand.hpp"
#include "Server.hpp"
#include "CliSocket.hpp"
#include "Channel.hpp"


bool isInChan(Channel *chan, std::string user)
{
	CliSocket *target = Command::findUserFd(user);
	if (target == NULL)
		return false;
	if (!target->isInList(chan->getMembers()))
		return false;
	return true;
}


void KickCommand::execute(const std::string &args, CliSocket *client)
{
	std::vector<std::string> vecArgs = Command::splitArgs(args);
	Channel *chan = chanExist(vecArgs[0]);
	CliSocket *target = Command::findUserFd(vecArgs[1]);


	if (!chan)
		return sendRpl(client, ERR_NOSUCHCHANNEL, client->getNick().c_str(), vecArgs[0].c_str());
	
	if (!isInChan(chan, vecArgs[1]))
		return sendRpl(client, ERR_USERNOTINCHANNEL, client->getNick().c_str(), vecArgs[1].c_str(), chan->getName().c_str());
	
	if (!isInChan(chan, client->getNick()))
		return sendRpl(client, ERR_NOTONCHANNEL, client->getNick().c_str(), chan->getName().c_str());

	if (!isChanOp(chan, client))
		return sendRpl(client, ERR_CHANOPRIVSNEEDED, client->getNick().c_str(), chan->getName().c_str());
	
	
	std::string msg = client->getSource() + " KICK " + chan->getName() + " " + target->getNick();
	if (vecArgs.size() == 3)
		msg = msg + " :" + vecArgs[2];
	broadcast(chan->getMembers(), msg);
	Channel::removeFromList(chan->getMembers(), target->getFd());
}

//   /connect 127.0.0.1 6657 0000