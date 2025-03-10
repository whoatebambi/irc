#pragma once


#include "Command.hpp"

class Client;


class CapCommand : public Command
{
    private:
       
    public:
        void execute(const std::string &args, Client *client);
};


