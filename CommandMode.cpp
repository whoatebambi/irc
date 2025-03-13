#include "CommandMode.hpp"

void CommandMode::execute(const std::string &args, Client *client)
{
	std::cout << "MODE command executed with args: " << args << std::endl;
	
	std::vector<std::string> arg = splitArgs(args);
	if (arg.size() < 1)
	{
		std::cout << "Not enough parameters" << std::endl;
		return;
		// return Command::sendRpl(client, ERR_NEEDMOREPARAMS, client->getNick().c_str(), " :Not enough parameters");
	}
	// std::string nickname = client->getNickname();
	if (arg[0] == client->getNickname())
	{
		std::cout << "Same nickname" << std::endl;
		// sendMsg(client, client->getNickname());
		// return Command::sendRpl(client, RPL_UMODEIS, client->getNick().c_str());
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