#pragma once

#include "Command.hpp"

class CliSocket;

class PingCommand : public Command
{
    private:

    public:
        void execute(const std::string &args, CliSocket *client);

};


