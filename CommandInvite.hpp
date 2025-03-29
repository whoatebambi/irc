#pragma once

#include "Command.hpp"
#include "Client.hpp"

#include <string>
#include <map>
#include <cctype>

class Client;

class CommandInvite : public Command
{
	public:
		void	execute(const std::string &args, Client *client);
};
