#pragma once


#include "Command.hpp"

class CliSocket;
class Channel;


class PartCommand : public Command
{
    private:
       
    public:
        void execute(const std::string &args, CliSocket *client);
        std::vector<Channel *> chanExist(std::vector<std::string> &chans, CliSocket *client);
};