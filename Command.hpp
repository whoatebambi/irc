#pragma once

#include <string>
#include <sstream>
#include <ctype.h>
#include <vector>
#include <set>

class Client;

class Command
{
	public:
		virtual	~Command() {}
		virtual	void execute(const std::string &args, Client *client) = 0;

		std::vector<std::string>	splitArgs(const std::string &input);
		std::vector<std::string>	splitString(const std::string &str, char delimiter);
};
