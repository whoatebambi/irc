#include "CliSocket.hpp"
#include "JoinCommand.hpp"
#include "Server.hpp"
#include "Channel.hpp"



bool isValid(const std::string &chan, bool key)
{
    if (!key)
    {
        if (chan.empty() || (chan[0] != '#' && chan[0] != '&'))
            return false;
    }
    for (size_t i = 1; i < chan.size(); ++i)
    {
        if (!isalnum(chan[i]))
            return false;
    }
    return true;
}


std::map<std::string, std::string> JoinCommand::parseArgs(const std::string &args, CliSocket *client)
{
    std::string arg = (args);
    std::map<std::string, std::string> toJoin;

    std::vector<std::string> vecArgs = splitArgs(arg);
    if (vecArgs.size() > 2)
        return (sendRpl(client, ERR_NEEDMOREPARAMS, client->getNick().c_str(), " :Too many parameters"), toJoin);

    std::vector<std::string> chans = splitString(vecArgs[0], ',');

    for (size_t i = 0; i < chans.size(); ++i)
    {
        if (!isValid(chans[i], false))
            return (sendRpl(client, ERR_BADCHANMASK, chans[i].c_str()), toJoin);
    }

    if (vecArgs.size() == 2)
    {
        std::vector<std::string> keys = splitString(vecArgs[1], ',');

        if (chans.size() != keys.size())
            return (sendRpl(client, ERR_NEEDMOREPARAMS, client->getNick().c_str(), " :Parameters size mismatch"), toJoin);

        for (size_t i = 0; i < keys.size(); ++i)
        {
            if (!isValid(keys[i], true))
                return (sendRpl(client, ERR_INVALIDKEY, client->getNick().c_str(), chans[i].c_str()), toJoin);
        }
        for (size_t i = 0; i < chans.size(); ++i)
        {
            toJoin[chans[i]] = keys[i];
        }
        return toJoin;
    }

	else
	{
        for (size_t i = 0; i < chans.size(); ++i)
        {
            toJoin[chans[i]] = "";
        }
        return toJoin;
	}
}

void JoinCommand::execute(const std::string &args, CliSocket *client)
{
    if (args.empty())
        return (sendRpl(client, ERR_NEEDMOREPARAMS, client->getNick().c_str()));

    std::map<std::string, std::string> argmap = parseArgs(args, client);

    if (argmap.empty())
        return;
        
    std::map<std::string, Channel*> OniChanUwU = Server::getInstance().getOniChanUwU();
    for (std::map<std::string, std::string>::const_iterator it = argmap.begin(); it != argmap.end(); ++it)
    {
        std::map<std::string, Channel*>::const_iterator it2 = OniChanUwU.begin();
        for (; it2 != OniChanUwU.end(); ++it2)
        {
            if (sameString(it->first, it2->first))
            {
                it2->second->joinChan(client, it->first, it->second);
                break;
            }
        }
        if (it2 == OniChanUwU.end() && it->first.size() > 1)      
            Server::getInstance().newChan(client, it->first, it->second);
        else
            return;
    }   
}
