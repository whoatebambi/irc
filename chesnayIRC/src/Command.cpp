#include "Command.hpp"
#include "Server.hpp"
#include "CliSocket.hpp"
#include "Channel.hpp"

ErrMsg Command::errMsg;


Command::~Command() {}

std::vector<std::string> Command::splitArgs(const std::string &input)
{
    std::vector<std::string> vec;
    std::istringstream iss(input);
    std::string word;
    //bool trailing = false;

    while(iss >> word)
    {
        if (word[0] == ':')
        {
            //trailing = true;
            vec.push_back(input.substr(input.find(':') + 1));
            break;
        }
        vec.push_back(word);
    }
    return vec;
    // (void)trailing;
}

void Command::sendMsg(CliSocket *client, std::string msg)
{
    std::cout << "SERVER To FD: " << client->getFd() << " >> " << msg << std::endl;
    msg += "\r\n";

    send(client->getFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
}

void Command::broadcast(std::set<int> fds, std::string msg)
{
    msg += "\r\n";
    for (std::set<int>::const_iterator it = fds.begin(); it != fds.end(); ++it)
    {
        std::cout << "SERVER To FD: " << *it << " >> " << msg;
        send(*it, msg.c_str(), msg.size(), MSG_NOSIGNAL);
    }
}

void Command::sendRpl(CliSocket *client, int err, ...)
{
    va_list args;
    va_start(args, err);
    std::string msg = errMsg.getErr(err, args);

    sendMsg(client, msg);
    va_end(args);
}

// void Command::sendRpl(CliSocket *client, int err, ...)
// {
//     std::cout << "sendRpl() called with error code: " << err << std::endl;

//     va_list args;
//     va_start(args, err);

//     // 🚀 Extract and print the arguments manually before calling getErr()
//     const char* arg1 = va_arg(args, const char*);
//     const char* arg2 = va_arg(args, const char*);
//     const char* arg3 = va_arg(args, const char*);

//     std::cout << "DEBUG - Arguments received in sendRpl:" << std::endl;
//     std::cout << "Arg1 (client nick): " << (arg1 ? arg1 : "nullptr") << std::endl;
//     std::cout << "Arg2 (chan name): " << (arg2 ? arg2 : "nullptr") << std::endl;
//     std::cout << "Arg3 (target): " << (arg3 ? arg3 : "nullptr") << std::endl;

//     // 🚀 Restart va_list before calling getErr()
//     va_end(args);
//     va_start(args, err);

//     std::string msg = errMsg.getErr(err, args);

//     va_end(args);

//     sendMsg(client, msg);
// }


void Command::sendWelcome(CliSocket *client)
{
    if (client->getNick().empty() || client->getUsrName().empty())
        return;
    
    std::string nick = client->getNick();

    sendMsg(client, "001 " + nick + " :Welcome to the IRC server, " + nick); // "<client> :Welcome to the <networkname> Network, <nick>[!<user>@<host>]"
    sendMsg(client, "002 " + nick + " :Your host is A VERY BIG PROUTEUR"); // add server name? decide on it
    sendMsg(client, "003 " + nick + " :This server was created just now"); // add a get time or smth to display the date of creation of serv. Not critical
    sendMsg(client, "004 " + nick + " " + "VERY BIG PROUTEUR" + " 1.0"); // -> list available channel / user modes
    sendMsg(client, "005 " + nick);
    sendMsg(client, "251 " + nick);
    sendMsg(client, "252 " + nick);
    sendMsg(client, "253 " + nick);
    sendMsg(client, "254 " + nick);
    sendMsg(client, "255 " + nick);
    sendMsg(client, "265 " + nick);
    sendMsg(client, "266 " + nick);
    sendMsg(client, "375 " + nick + " :- MESSAGE OF THE DAY -");
    sendMsg(client, "372 " + nick + " :- I WISH YOU A MERRY PROUTING DAY -");
    sendMsg(client, "376 " + nick + " :- THATS IT -");
    sendMsg(client, "251 " + nick);

    std::cout << "SENT WELCOME" << std::endl;
}



void Command::sendTopic(CliSocket *client, Channel *channel)
{
        if(channel->getTopic().empty())
        {
            std::string msg = "331 " + client->getNick() + " " + channel->getName() + " :No topic is set";
            sendMsg(client, msg);
        }
        else
        {
            std::string msg_Topic = "332 " + client->getNick() + " " + channel->getName() + " " + channel->getTopic();
            sendMsg(client, msg_Topic);
            std::string msg_whoTime = "333 " + client->getNick() + " " + channel->getName() + " " + channel->getTopicEditor() + " " + channel->getTopicTime();
            sendMsg(client, msg_whoTime);
            std::cout << "AAAAAAAAAAAAAAAA" << std::endl;
        }
}

std::vector<std::string> Command::splitString(const std::string &str, char delimiter) 
{
    std::vector<std::string> result;
    std::string item;
    std::stringstream ss(str);

    if (!str.empty() && str[0] == delimiter)
        result.push_back("");

    while (std::getline(ss, item, delimiter))
        result.push_back(item);

    if (!str.empty() && str[str.length() - 1] == delimiter)
        result.push_back("");
 
    return result;
}

bool Command::isChanOp(Channel *chan, CliSocket *client)
{
	if (!client->isInList(chan->getOpsList()))
		return false;
	return true;
}

CliSocket *Command::findUserFd(std::string user)
{
	std::vector<CliSocket *> clients = Server::getInstance().getClients();
	for (std::vector<CliSocket *>::const_iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (sameString(user, (*it)->getNick()))
			return *it;
	}
	return NULL;
}

Channel *Command::chanExist(std::string chanName)
{
	Channel *chan;

	std::map<std::string, Channel*> OniChanUwU = Server::getInstance().getOniChanUwU();
	std::map<std::string, Channel*>::iterator it2 = OniChanUwU.begin();
	for( ; it2 != OniChanUwU.end(); ++it2)
	{
		if (sameString(chanName, it2->first))
		{
			chan = it2->second;
			return chan;
		}
	}
	return NULL;
}

CliSocket *Command::findClientFromNick(std::string nick)
{
    std::vector<CliSocket *> vecClients = Server::getInstance().getClients();
    std::vector<CliSocket *>::const_iterator it = vecClients.begin();

    for ( ; it != vecClients.end(); ++it)
    {
        if (sameString((*it)->getNick(), nick))
            break ;
    }
    if (it == vecClients.end())
        return NULL;
    return (*it);
}

