#include "CommandUser.hpp"

void CommandUser::execute(const std::string &args, Client *client)
{
	std::cout << INVERSE_BG << BLUE << "USER command executed with args: " BOLD << args << RESET << std::endl;

	std::vector<std::string> arg = splitArgs(args);
	if (arg.size() < 4)
		return sendRpl(client->getFd(), "ERR_NEEDMOREPARAMS", client->getNickname().c_str());
	if (!(client->getUsername().empty()))
		return sendRpl(client->getFd(), "ERR_ALREADYREGISTERED", client->getNickname().c_str());

	client->setUsername(arg[0]);
	client->setRealname(arg[3]);
}

std::vector<std::string> CommandUser::splitArgs(const std::string &input)
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

void CommandUser::sendRpl(int fd, std::string err, ...)
{
	va_list args;
	va_start(args, err);

	std::string msg = "CommandUser::sendRpl(error)";
	msg += "\r\n";

	send(fd, msg.c_str(), msg.size(), MSG_NOSIGNAL);
	va_end(args);
}
