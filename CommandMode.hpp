#pragma once

#include "Command.hpp"
#include "Client.hpp"

#include <string>

class Client;

class CommandMode : public Command
{
	public:
		void execute(const std::string &args, Client *client);
		std::vector<std::string>	splitArgs(const std::string &input);
};
