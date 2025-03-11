#pragma once

#include "Client.hpp"

#include <string>

class Client;

class Command {
    public:
		virtual ~Command() {}
		virtual void execute(const std::string &args, Client *client) = 0;

		static void sendMsg(Client *client, std::string msg);
};
