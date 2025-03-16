#include "Command.hpp"
#include "Client.hpp"

void Command::sendMsg(Client *client, std::string msg)
{
	std::cout << INVERSE_BG << RED << ">>> " << BOLD << msg << RESET << std::endl;
	msg += "\r\n";
	send(client->getFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
}

void Command::sendRpl(Client *client, int err, std::string msg)
{
	std::stringstream ss;
    ss << err << ": " << msg;
    std::string msg2 = ss.str();
	// std::cout << "msg2 = " << msg2 << std::endl;
    sendMsg(client, msg2);
}

void Command::broadcast(std::set<int> fds, std::string msg)
{
    msg += "\r\n";
    for (std::set<int>::const_iterator it = fds.begin(); it != fds.end(); ++it)
    {
        std::cout << INVERSE_BG << RED << ">>> " << BOLD << "SERVER to FD<" << *it << ">" << RESET << msg;
        send(*it, msg.c_str(), msg.size(), MSG_NOSIGNAL);
    }
}

std::vector<std::string> Command::splitArgs(const std::string &input)
{
	std::vector<std::string> vec;
	std::istringstream iss(input);
	std::string word;

	while(iss >> word)
	{
		// if (word[0] == ':')
		// {
		// 	vec.push_back(input.substr(input.find(':') + 1));
		// 	break;
		// }
		vec.push_back(word);
	}
	return vec;
}

std::vector<std::string> Command::splitString(const std::string &str, char delimiter) 
{
    std::vector<std::string> result;
    std::string item;
    std::stringstream ss(str);

    if (!str.empty() && str[0] == delimiter)
        result.push_back("");

    while (std::getline(ss, item, delimiter))
        result.push_back(item);

    if (!str.empty() && str[str.length() - 1] == delimiter)
        result.push_back("");
 
    return result;
}