#include "CommandMode.hpp"

void CommandMode::execute(const std::string &args, Client *client)
{
	std::cout << INVERSE_BG << BLUE << "MODE command executed with args: " BOLD << args << RESET << std::endl;
	
	std::vector<std::string> arg = splitArgs(args);
	if (arg.size() < 1)
	{
		std::cout << "Not enough parameters" << std::endl;
		return;
	}
	if (arg[0] == client->getNickname())
	{
		std::string serverName = Server::getInstance().getServerName();
		std::string nickname = client->getNickname();
		std::string msg = ":" + serverName + " 221 " + nickname + " +i";
		std::cout << "msg = " << msg << std::endl;
		sendMsg(client, msg.c_str());
	}
}

std::vector<std::string> CommandMode::splitArgs(const std::string &input)
{
	std::vector<std::string> vec;
	std::istringstream iss(input);
	std::string word;
	//bool trailing = false;

	while(iss >> word)
	{
		if (word[0] == ':')
		{
			//trailing = true;
			vec.push_back(input.substr(input.find(':') + 1));
			break;
		}
		vec.push_back(word);
	}
	return vec;
	// (void)trailing;
}