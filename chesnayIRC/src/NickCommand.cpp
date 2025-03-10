#include "../include/CliSocket.hpp"
#include "../include/NickCommand.hpp"
#include "Server.hpp"
#include <set>

void NickCommand::execute(const std::string &args, CliSocket *client)
{   
    std::string arr = "[]{}\\|";

    if (!client->isAuth())
    {
        sendRpl(client, ERR_NOTREGISTERED, client->getNick().c_str());
        Server::getInstance().removeClient(client);
        return;
    }
    if (args.empty())
        return sendRpl(client, ERR_NONICKNAMEGIVEN, client->getNick().c_str());

    for (size_t i = 0; i < args.length(); ++i)
    {
        if (!isalnum(args[i]) && (arr.find(args[i]) == std::string::npos))
            return sendRpl(client, ERR_ERRONEUSNICKNAME, client->getNick().c_str(), args.c_str());
    }
    if (Server::getInstance().nickTaken(args))
    {
        if (client->getNick().empty())
        {
            sendRpl(client, ERR_NICKNAMEINUSE, args.c_str(), args.c_str());
            sendMsg(client, "ERROR :Retry with another Nickname");
            Server::getInstance().removeFromFdList(client->getFd());
            return;
        }
        else
            return sendRpl(client, ERR_NICKNAMEINUSE, client->getNick().c_str(), args.c_str());
    }
        
    std::string oldnick = client->getNick();
    const std::string oldmask = client->getSource();
    client->setNick(args);
    const std::string newmask = client->getSource();

    Server::getInstance().updateList(oldmask, newmask);

    std::set<int> pairs = Server::getInstance().getSharedChans(client);
    broadcast(pairs, ":" + oldnick + " NICK " + args);
}
