#pragma once

#include "Command.hpp"
#include "Client.hpp"

#include <string>
#include <map>
#include <cctype>

class Client;

class CommandJoin : public Command
{
	public:
		void	execute(const std::string &args, Client *client);
		std::map<std::string, std::string>	parseArgs(const std::string &args, Client *client);
};

// void	replaceDefaultKey(std::vector<std::string> &keys);
bool	isValidChannelString(const std::string &channel);
bool	isValidKeyString(const std::string &key);
bool	sameString(const std::string &s1, const std::string &s2);
std::string toLower(const std::string &str);