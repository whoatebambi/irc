#pragma once

#include <string>

class Client;

class Command {
    private:

    public:
		virtual ~Command() {}
		virtual void execute(const std::string &args, Client *client) = 0;

		static void sendMsg(Client *client, std::string msg);
};
