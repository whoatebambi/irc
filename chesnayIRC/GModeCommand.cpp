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
    CliSocket *target = Command::findClientFromNick(nick);
    if (target != NULL)
        channel.addBanList(target, client);
    else
    {
        std::set<std::string> banList = channel.getBanList();
        for (std::set<std::string>::const_iterator it = banList.begin(); it != banList.end(); ++it)
            Command::sendRpl(client, RPL_NAMREPLY, client->getNick().c_str(), channel.getName().c_str(), *it->c_str());
        Command::sendRpl(client, RPL_ENDOFNAMES, client->getNick().c_str(), channel.getName().c_str());
    }
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
    // Create a map of mode commands to function pointers
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
    //rajouter +b -b plus tard
	// and so on for -o +l etc..

    // Iterate through mode changes and execute the corresponding function
    for (size_t i = 0; i < modeChanges.size(); ++i) 
	{
        std::map<std::string, ModeFunction>::iterator it = modeHandlers.find(modeChanges[i].first);

        it->second(channel, modeChanges[i].second, client);
    }
}


bool checkValidMode(const std::string& modeStr)
{
    const std::string validChars = "olkitb+-";
    bool hasValidMode = false;

    for (size_t i = 0; i < modeStr.length(); ++i)
	{
        char c = modeStr[i];

        if (validChars.find(c) == std::string::npos)
            return false;
        if (c == 'o' || c == 'l' || c == 'k' || c == 'i' || c == 't' || c =='b')
            hasValidMode = true;
    }
    return hasValidMode;
}

void returnMode(Channel *chan, CliSocket *client)
{
    std::string mString = chan->modeString();
    std::string creationTime = chan->getCreationTime();
    std::string key = chan->getKey();
    std::string limitSize = chan->getLimitSize();
    //     :irc.example.com 324 user456 #channel1 +bkol secretkey 25
    // :irc.example.com 329 user456 #channel1 1620835678
    Command::sendRpl(client, RPL_CHANNELMODEIS, client->getNick().c_str(), chan->getName().c_str(), mString.c_str(), key.c_str(), limitSize.c_str());
    Command::sendRpl(client, RPL_CREATIONTIME, client->getNick().c_str() ,chan->getName().c_str(), creationTime.c_str());
    // :irc.example.com 367 user456 #channel1 baduser!*@*
    // :irc.example.com 367 user456 #channel1 *!*@spammer.net
    // :irc.example.com 368 user456 #channel1 :End of channel ban list
    if (mString.find('b') != std::string::npos)
    {
        std::set<std::string> banList = chan->getBanList();
        for (std::set<std::string>::const_iterator it = banList.begin(); it != banList.end(); ++it)
            Command::sendRpl(client, RPL_NAMREPLY, client->getNick().c_str(), chan->getName().c_str(), *it->c_str());
        Command::sendRpl(client, RPL_ENDOFNAMES, client->getNick().c_str(), chan->getName().c_str());
    }
    // :irc.example.com 353 user123 = #channel1 :@AdminUser @ModeratorUser +HelperUser RegularUser1 RegularUser2 RegularUser3
    // :irc.example.com 366 user123 #channel1 :End of NAMES list
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
        // Techniquement on doit ajouter la liste des membre normaux mais flemme
        // std::set<int> cliList = chan->getMembers();
        // for (std::set<int>::const_iterator it = cliList.begin(); it != cliList.end(); ++it)
        // {
        //     if()
        //     list += *it;
        //     list += " ";
        // }
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
            else//send error
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
        // if (!checkValidMode(args[1]))  //is this needed ????????????
        // {
        //     //send error
        //     return ;
        // }

        std::cout << "11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111" << std::endl;
        std::string modeString = args[1]; // the "+ooo-l+i" string, need to check if it contains invalid chars
    
        std::cout << "" << std::endl;
        std::vector<std::string> modeArgs(args.begin() + 2, args.end()); // all the other strings that are the effective arguments ex : "nickname" + "50"
    
        // the string of modes formatted to always include the sign. For examples "+ooo" -> "+o+o+o" | "-li+ok" -> "-l-i+o+k"
    
        std::vector<std::pair<std::string, std::string> > modeChanges; // a vector of pairs mode + arg. Arg can be empty for i or t. Ex: vector = { [+o, nickname] [+i, ""] [+l, 55] }
    
        size_t argIndex = 0;
    
        char sign = '+'; // Default to adding mode
    
        for (size_t i = 0; i < modeString.size(); ++i) // iterate through  "+ooo-l+i"
        {
            std::string appliedModes = "";
            char mode = modeString[i]; // store the character
    
            if (mode == '+' || mode == '-') // if the char is a sign
            {
                sign = mode; // change the current sign to this sign char
                continue;
            }
    
            // Modes that require an argument ADDED -b and +b need to do checks
            if ((mode == 'o' || mode == 'b' || ((mode == 'k' || mode == 'l') && sign == '+')) && argIndex < modeArgs.size())
            {
                appliedModes += sign;
                appliedModes += mode;
                modeChanges.push_back(std::make_pair(appliedModes, modeArgs[argIndex++])); //what does it do if MODE #chan1 +b ? does it add a "" to modeChanges
            } 
            // Modes that don't require an argument
            else if (mode == 'i' || mode == 't' || ((mode == 'k' || mode == 'l') && sign == '-'))
            {
                appliedModes += sign;
                appliedModes += mode;
                modeChanges.push_back(std::make_pair(appliedModes, ""));
            }
            else if (mode == 'b' && modeArgs.empty())
            {
                appliedModes += sign;
                appliedModes += mode;
                std::cout << "BLABLAVLABLABLAVLABLABLAVLABLABLAVLABLABLAVLABLABLAVLABLABLAVLABLABLAVLA" << std::endl;
                modeChanges.push_back(std::make_pair(appliedModes, "")); //what does it do if MODE #chan1 +b ? does it add a "" to modeChanges
            }
                
            else
            {
                std::cout << "ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ" << std::endl;
                std::string str(1, mode);
                Command::sendRpl(client, ERR_UNKNOWNMODE, client->getNick().c_str(), str.c_str());
            }
        }
        
        // Debug Output
        //std::cout << "Applied Modes: " << appliedModes << "\n";
        //std::cout << "Mode Changes:\n";
        // for (size_t i = 0; i < modeChanges.size(); ++i) 
        // {
        //     std::cout << "  " << " mode " << modeChanges[i].first;
        //     if (!modeChanges[i].second.empty()) std::cout << " with arg: " << modeChanges[i].second;
        //     std::cout << "\n";
        // }
        processModeChanges(*chan, modeChanges, client);
    }
}

void ModeCommand::execute(const std::string &args, CliSocket *client)
{
	handleModeCommand(splitArgs(args), client);
}


// args = +o -o +l +k 
// noargs = +i -i +t -t -k -l -b
// depends = +b


// if (args && )