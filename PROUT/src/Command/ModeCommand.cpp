#include "CliSocket.hpp"
#include "ModeCommand.hpp"
#include "Server.hpp"
#include "Channel.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <utility>

void modeAddOp(Channel &channel, const std::string &nick, CliSocket *client)
{
    CliSocket *target = Command::findClientFromNick(nick);
    if (target != NULL)
        channel.addOp(target->getSource(),target , client);
    else
        return Command::sendRpl(client, ERR_NOSUCHNICK, client->getNick().c_str(), target->getNick().c_str());
}

void modeDelOp(Channel &channel, const std::string &nick, CliSocket *client)
{
    CliSocket *target = Command::findClientFromNick(nick);
    if (target != NULL)
        channel.delOp(target->getSource(), target, client);
    else
        return Command::sendRpl(client, ERR_NOSUCHNICK, client->getNick().c_str(), target->getNick().c_str());
}

/////////////////////////////////////////////////////////////

void modeAddLimit(Channel &channel, const std::string &arg, CliSocket *client)
{
    for (size_t i = 0; i < arg.length(); ++i)
    {
        if (!isdigit(arg[i]))
            return Command::sendRpl(client, ERR_UNKNOWNMODE, client->getNick().c_str(), arg.c_str());
    }
    int lim = strtol(arg.c_str(), NULL, 10);
    channel.addLimit(lim, client);
}

void modeDelLimit(Channel &channel, const std::string &emptyarg, CliSocket *client) 
{
    if (emptyarg != "")
        return Command::sendRpl(client, ERR_UNKNOWNMODE, client->getNick().c_str(), emptyarg.c_str());
    channel.delLimit(client);
}

/////////////////////////////////////////////////////////////

void modeAddKey(Channel &channel, const std::string &key, CliSocket *client) 
{
    //peut etre add des checks
    if (key == "")
    {
        //ne rien faire ou peut etre erreur jsp
        return ;
    }
    channel.addKey(key, client);
}

void modeDelKey(Channel &channel, const std::string &emptyarg, CliSocket *client)
{
    if (emptyarg != "")
        return Command::sendRpl(client, ERR_UNKNOWNMODE, client->getNick().c_str(), emptyarg.c_str());
    channel.delKey(client);
}

/////////////////////////////////////////////////////////////

void modeAddInvOnly(Channel &channel, const std::string &emptyarg, CliSocket *client) //faire une fonction pour les deux maybe
{
    if (emptyarg != "")
        return Command::sendRpl(client, ERR_UNKNOWNMODE, client->getNick().c_str(), emptyarg.c_str());
    channel.addInvOnly(client);
}

void modeDelInvOnly(Channel &channel, const std::string &emptyarg, CliSocket *client)
{
    if (emptyarg != "")
        return Command::sendRpl(client, ERR_UNKNOWNMODE, client->getNick().c_str(), emptyarg.c_str());
    channel.delInvOnly(client);
}

/////////////////////////////////////////////////////////////

void modeAddNeedOpTopic(Channel &channel, const std::string &emptyarg, CliSocket *client)
{
    if (emptyarg != "")
        return Command::sendRpl(client, ERR_UNKNOWNMODE, client->getNick().c_str(), emptyarg.c_str());
    channel.addNeedOpTopic(client);
}

void modeDelNeedOpTopic(Channel &channel, const std::string &emptyarg, CliSocket *client)
{
    if (emptyarg != "")
        return Command::sendRpl(client, ERR_UNKNOWNMODE, client->getNick().c_str(), emptyarg.c_str());
    channel.delNeedOpTopic(client);
}


/////////////////////////////////////////////////////////////


void modeAddBanList(Channel &channel, const std::string &nick, CliSocket *client)
{
    if (nick.empty())
    {
        std::set<std::string> banList = channel.getBanList();
        for (std::set<std::string>::const_iterator it = banList.begin(); it != banList.end(); ++it)
        {
            std::string temp = *it;
            Command::sendRpl(client, RPL_NAMREPLY, client->getNick().c_str(), channel.getName().c_str(), temp.c_str());
        }
        return Command::sendRpl(client, RPL_ENDOFNAMES, client->getNick().c_str(), channel.getName().c_str());
    }
    CliSocket *target = Command::findClientFromNick(nick);
    if (target == NULL)
        return Command::sendRpl(client, ERR_NOSUCHNICK, client->getNick().c_str(), nick.c_str());
    channel.addBanList(target, client);
}

void modeDelBanList(Channel &channel, const std::string &nick, CliSocket *client)
{
    CliSocket *target = Command::findClientFromNick(nick);
    if (target != NULL)
        channel.delBanList(target, client);
    else
        return Command::sendRpl(client, ERR_NOSUCHNICK, client->getNick().c_str(), target->getNick().c_str());
}

void processModeChanges(Channel& channel, const std::vector<std::pair<std::string, std::string> >& modeChanges, CliSocket *client) 

{

    std::map<std::string, ModeFunction> modeHandlers;
	modeHandlers["+o"] = &modeAddOp;
    modeHandlers["-o"] = &modeDelOp;

    modeHandlers["+l"] = &modeAddLimit;
    modeHandlers["-l"] = &modeDelLimit;

    modeHandlers["+k"] = &modeAddKey;
    modeHandlers["-k"] = &modeDelKey;

    modeHandlers["+i"] = &modeAddInvOnly;
    modeHandlers["-i"] = &modeDelInvOnly;

    
    modeHandlers["+t"] = &modeAddNeedOpTopic;
    modeHandlers["-t"] = &modeDelNeedOpTopic;

    modeHandlers["+b"] = &modeAddBanList;
    modeHandlers["-b"] = &modeDelBanList;

    for (size_t i = 0; i < modeChanges.size(); ++i) 
	{
        std::map<std::string, ModeFunction>::iterator it = modeHandlers.find(modeChanges[i].first);

        it->second(channel, modeChanges[i].second, client);
    }
}


bool checkValidMode(const char& c)
{
    const std::string validChars = "olkitb";

    if (validChars.find(c) == std::string::npos)
        return false;
    return true;
}

void returnMode(Channel *chan, CliSocket *client)
{
    std::string mString = chan->modeString();
    std::string creationTime = chan->getCreationTime();
    std::string key = chan->getKey();
    std::string limitSize = chan->getLimitSize();

    Command::sendRpl(client, RPL_CHANNELMODEIS, client->getNick().c_str(), chan->getName().c_str(), mString.c_str(), key.c_str(), limitSize.c_str());
    Command::sendRpl(client, RPL_CREATIONTIME, client->getNick().c_str() ,chan->getName().c_str(), creationTime.c_str());

    if (mString.find('b') != std::string::npos)
    {
        std::set<std::string> banList = chan->getBanList();
        for (std::set<std::string>::const_iterator it = banList.begin(); it != banList.end(); ++it)
        {
            std::string temp = *it;
            Command::sendRpl(client, RPL_NAMREPLY, client->getNick().c_str(), chan->getName().c_str(), temp.c_str());
        }
        Command::sendRpl(client, RPL_ENDOFNAMES, client->getNick().c_str(), chan->getName().c_str());
    }
    if (mString.find('o') != std::string::npos)
    {
        std::string list =":";
        std::set<std::string> opeList = chan->getOpsList();
        for (std::set<std::string>::const_iterator it = opeList.begin(); it != opeList.end(); ++it)
        {
            list += "@";
            list += *it;
            list += " ";
        }
        Command::sendRpl(client, RPL_BANLIST, client->getNick().c_str(), chan->getName().c_str(), list.c_str());
        Command::sendRpl(client, RPL_ENDOFBANLIST, client->getNick().c_str(), chan->getName().c_str());
    }
}

void handleModeCommand(const std::vector<std::string>& args, CliSocket *client) 
{
    if (args.size() < 1)
        return Command::sendRpl(client, ERR_NEEDMOREPARAMS, client->getNick().c_str(), " :Not enough parameters");
    if (args[0] == client->getNick())
        return Command::sendRpl(client, RPL_UMODEIS, client->getNick().c_str());
    else
    {
        Channel *chan = Command::chanExist(args[0]);
        if (!chan)
        {
            if (args[0][0] == '#')
            {
                return Command::sendRpl(client, ERR_NOSUCHCHANNEL, client->getNick().c_str(), args[0].c_str());
            }
            else
            {
                std::vector<CliSocket *>::iterator it;
                std::vector<CliSocket *> ClientList = Server::getInstance().getClients();
                for (it = ClientList.begin(); it != ClientList.end(); ++it) 
                {
                    if (*it != NULL && (*it)->getNick() == args[0]) 
                        return Command::sendRpl(client, ERR_NOSUCHNICK, client->getNick().c_str(), args[0].c_str());
                }
                return Command::sendRpl(client, ERR_USERSDONTMATCH, client->getNick().c_str());
            }
        }
        if (args.size() == 1)
        {
            returnMode(chan, client);
            return;
        }
        if (client->getSource() != chan->getFounderMask() && !client->isInList(chan->getOpsList()))
            return Command::sendRpl(client, ERR_CHANOPRIVSNEEDED, client->getNick().c_str(), chan->getName().c_str());
        
        std::string modeString = args[1];
        std::vector<std::string> modeArgs(args.begin() + 2, args.end());
        std::vector<std::pair<std::string, std::string> > modeChanges;
        size_t argIndex = 0;
        char sign = '+';

        for (size_t i = 0; i < modeString.size(); ++i)
        {
            std::string appliedModes = "";
            char mode = modeString[i];
    
            if (mode == '+' || mode == '-')
            {
                sign = mode;
                continue;
            }
    
            if ((mode == 'o' || mode == 'b' || ((mode == 'k' || mode == 'l') && sign == '+')) && argIndex < modeArgs.size())
            {
                appliedModes += sign;
                appliedModes += mode;
                modeChanges.push_back(std::make_pair(appliedModes, modeArgs[argIndex++]));
            } 

            else if (mode == 'i' || mode == 't' || ((mode == 'k' || mode == 'l') && sign == '-') || (mode == 'b' && sign == '+'))
            {
                appliedModes += sign;
                appliedModes += mode;
                modeChanges.push_back(std::make_pair(appliedModes, ""));
            }
            else if (!checkValidMode(mode))
            {
                std::string str(1, mode);
                Command::sendRpl(client, ERR_UNKNOWNMODE, client->getNick().c_str(), str.c_str());
            }
            else
                Command::sendRpl(client, ERR_NEEDMOREPARAMS, client->getNick().c_str(), " MODE: Not enough parameters");
        }
        processModeChanges(*chan, modeChanges, client);
    }
}

void ModeCommand::execute(const std::string &args, CliSocket *client)
{
	handleModeCommand(splitArgs(args), client);
}
