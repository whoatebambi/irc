#pragma once


#include "Command.hpp"

class CliSocket;


class NickCommand : public Command
{
    private:
       
    public:
        void execute(const std::string &args, CliSocket *client);
};