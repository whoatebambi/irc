#pragma once

#include "Command.hpp"
#include "CapCommand.hpp"
#include "PassCommand.hpp"
#include "NickCommand.hpp"
#include "UserCommand.hpp"
#include "PingCommand.hpp"
#include "JoinCommand.hpp"
#include "QuitCommand.hpp"
#include "PartCommand.hpp"
#include "KickCommand.hpp"
#include "InviteCommand.hpp"
#include "ModeCommand.hpp"
#include "TopicCommand.hpp"
#include "PrivmsgCommand.hpp"
#include <map>

class CliSocket;

class Factory
{
private:
    std::map<std::string, Command*> _commands;
    CliSocket* _client;

public:
    Factory(CliSocket *client);
    ~Factory();

    std::string ft_trim(const std::string& str);
    void parse(std::string &line);
};


