#pragma once

#include "Client.hpp"
#include "Command.hpp"

class CliSocket;

class CommandQuit : public Command
{
    private:

    public:
        void execute(const std::string &args, Client *client);
};
