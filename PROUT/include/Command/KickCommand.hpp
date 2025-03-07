#pragma once


#include "Command.hpp"

class CliSocket;
class Channel;


class KickCommand : public Command
{
    private:
       
    public:
        void execute(const std::string &args, CliSocket *client);
};