#pragma once

#include "Command.hpp"

class CliSocket;

typedef void (*ModeFunction)(Channel&, const std::string&, CliSocket*);

class ModeCommand : public Command
{
    private:
       
    public:
        void execute(const std::string &args, CliSocket *client);

};