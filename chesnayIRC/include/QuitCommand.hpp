#pragma once

#include "Command.hpp"

class CliSocket;

class QuitCommand : public Command
{
    private:

    public:
        void execute(const std::string &args, CliSocket *client);
};

