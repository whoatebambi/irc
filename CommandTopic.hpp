#pragma once

#include "Command.hpp"
#include "Client.hpp"

#include <string>
#include <vector>

class Client;

class CommandTopic : public Command
{
	public:
		void	execute(const std::string &args, Client *client);
};

bool	isValidChannelTopic(const std::string &topic);
