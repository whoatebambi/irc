#include "CliSocket.hpp"
#include "TopicCommand.hpp"
#include "Server.hpp"
#include "Channel.hpp"

void TopicCommand::execute(const std::string &args, CliSocket *client)
{
    if(args.empty())
        return sendRpl(client, ERR_NEEDMOREPARAMS, client->getNick().c_str(), " TOPIC :Not enough parameters");
    
    std::vector<std::string> vecArgs = splitArgs(args);

    if(vecArgs.size() > 2)
        return sendRpl(client, ERR_NEEDMOREPARAMS, client->getNick().c_str(), " TOPIC :Too many parameters");

    std::map<std::string, Channel*> OniChanUwU = Server::getInstance().getOniChanUwU();
    std::map<std::string, Channel*>::iterator it = OniChanUwU.begin();
    for (; it != OniChanUwU.end(); ++it)
    {
        if (sameString(vecArgs[0], it->first))
        {
            break;            
        }
    }
    if (it == OniChanUwU.end())
        sendRpl(client, ERR_NOSUCHCHANNEL, client->getNick().c_str(), vecArgs[0].c_str());
    if (!client->isInList(it->second->getMembers()))
        sendRpl(client, ERR_NOTONCHANNEL, client->getNick().c_str(), it->second->getName().c_str());

    if(vecArgs.size() == 1)
    {
        std::cout << "HELLO MY NEW FRIEND" << std::endl;
        Command::sendTopic(client,it->second);
    }
    else
    {
        if (it->second->checkMode('t') && !client->isInList(it->second->getOpsList())) 
            sendRpl(client, ERR_CHANOPRIVSNEEDED, client->getNick().c_str(), it->second->getName().c_str());

        it->second->setTopicInfo(vecArgs[1], client->getNick());
        std::string msg = client->getSource() + " TOPIC " + it->second->getName() + " :" + it->second->getTopic();
        broadcast(it->second->getMembers(), msg);
    }
}
