#pragma once


#include "Command.hpp"

class CliSocket;


class PassCommand : public Command
{
    private:
       
    public:
        void execute(const std::string &args, CliSocket *client);
};