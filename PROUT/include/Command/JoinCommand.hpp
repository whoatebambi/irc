#pragma once


#include "Command.hpp"

class CliSocket;


class JoinCommand : public Command
{
    private:
       
    public:
        void execute(const std::string &args, CliSocket *client);
        std::map<std::string, std::string> parseArgs(const std::string &args, CliSocket *client);
};

