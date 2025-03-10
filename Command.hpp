#pragma once

#include <string>

class Client;

class Command
{
    private:

    public:
        virtual ~Command() = 0;
        virtual void execute(const std::string &args, Client *client) = 0;
        
        static void sendMsg(Client *client, std::string msg);
        // void sendWelcome(Client *client);
        // static void sendRpl(Client *client, int err, ...);
        // static void sendTopic(Client *client, Channel *channel);
        // static void broadcast(std::set<int> fds, std::string msg);
};

