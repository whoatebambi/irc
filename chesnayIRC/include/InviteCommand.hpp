#pragma once

#include "Command.hpp"

class CliSocket;

class InviteCommand : public Command
{
    private:

    public:
        void execute(const std::string &args, CliSocket *client);

};