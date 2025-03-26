#pragma once

#include "Command.hpp"
#include "Client.hpp"

#include <string>
#include <vector>

class Client;

class CommandUser : public Command
{
	public:
		void	execute(const std::string &args, Client *client);
};
