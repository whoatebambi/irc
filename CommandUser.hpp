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
		std::vector<std::string>	splitArgs(const std::string &input);
		void	sendRpl(int fd, std::string err, ...);
};
