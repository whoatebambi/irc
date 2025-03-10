#pragma once

#include "Command.hpp"

class CliSocket;

class UserCommand : public Command
{
    public:
        void execute(const std::string &args, CliSocket *client);
};