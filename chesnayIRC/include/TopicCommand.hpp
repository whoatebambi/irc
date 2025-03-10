#pragma once

#include "Command.hpp"

class CliSocket;

class TopicCommand : public Command
{
    private:

    public:
        void execute(const std::string &args, CliSocket *client);
};