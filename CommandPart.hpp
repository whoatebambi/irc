#pragma once

#include "Command.hpp"
#include "Client.hpp"

#include <string>
#include <map>
#include <cctype>

class Client;

class CommandPart : public Command
{
	public:
		void	execute(const std::string &args, Client *client);
		std::map<std::string, std::string>	parseArgs(const std::string &args, Client *client);
};

bool isValidPartMsg(const std::string &msg);