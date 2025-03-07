#include "CliSocket.hpp"
#include "InviteCommand.hpp"
#include "Server.hpp"
#include "Channel.hpp"


void InviteCommand::execute(const std::string &args, CliSocket *client)
{
    std::vector<std::string> vecArgs = splitArgs(args);
    
    if(vecArgs.size() != 2)
        sendRpl(client, ERR_NEEDMOREPARAMS, client->getNick().c_str());

    std::map<std::string, Channel*> OniChanUwU = Server::getInstance().getOniChanUwU();
    std::map<std::string, Channel*>::iterator it = OniChanUwU.begin();

    for (; it != OniChanUwU.end(); ++it)
    {
        if (sameString(vecArgs[1], it->first))
        {
            break;            
        }
    }

    if (it == OniChanUwU.end())
        return sendRpl(client, ERR_NOSUCHCHANNEL, client->getNick().c_str(), vecArgs[1].c_str());
    
    if (!client->isInList(it->second->getMembers())) 
        return sendRpl(client, ERR_NOTONCHANNEL, client->getNick().c_str(), it->second->getName().c_str());
    
    if (it->second->checkMode('i') && !client->isInList(it->second->getOpsList()))
        return sendRpl(client, ERR_CHANOPRIVSNEEDED, client->getNick().c_str(), it->second->getName().c_str());

    CliSocket *target = Command::findUserFd(vecArgs[0]);
    if(target == NULL)
        return sendRpl(client, ERR_NOSUCHNICK, client->getNick().c_str(), vecArgs[0].c_str());
    
    it->second->getWhiteList().insert(target->getSource().c_str());

    sendRpl(client, RPL_INVITING, client->getNick().c_str(), vecArgs[0].c_str(), vecArgs[1].c_str());
    sendMsg(target, client->getSource() + " INVITE " + vecArgs[0] + " " + vecArgs[1]);
}

