#pragma once


#include "Command.hpp"

class CliSocket;

class PrivmsgCommand : public Command
{
    private:
       
    public:
        void execute(const std::string &args, CliSocket *client);
};
