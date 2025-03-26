#pragma once

#include "Command.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"

class Client;

class CommandPrivMsg : public Command
{
	public:
		void execute(const std::string &args, Client *client);
};
