#pragma once

#include "Command.hpp"
#include "Client.hpp"

#include <string>

class Client;

class CommandPing : public Command
{
	public:
		void execute(const std::string &args, Client *client);
};
