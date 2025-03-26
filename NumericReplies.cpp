#include "NumericReplies.hpp"

// Error code descriptions
static std::map<int, std::string> errorMessages;

void  NumericReplies::initErrorMessages()
{
	errorMessages.insert(std::make_pair(ERR_NEEDMOREPARAMS, "Not enough parameters"));
	errorMessages.insert(std::make_pair(ERR_BADCHANMASK, "Bad Channel Mask"));
	errorMessages.insert(std::make_pair(ERR_NOSUCHCHANNEL, "No such channel"));
	errorMessages.insert(std::make_pair(ERR_CHANOPRIVSNEEDED, "You're not channel operator"));
}

// Get error message from map
std::string getErrorMessage(int code)
{
	std::map<int, std::string>::const_iterator it = errorMessages.find(code);
	if (it != errorMessages.end())
		return " :" + it->second;
	return "";
	// return "Unknown Error";
}

void NumericReplies::sendNumReply(Client *client, int numericCode)
{
	if (!client)
		return;

	std::ostringstream msg;
	msg << ":" << Server::getInstance().getServerName() << " " << numericCode << " " << client->getNickname();
	msg << getErrorMessage(numericCode) << "\r\n";

	std::cout << INVERSE_BG << RED << ">>> " << BOLD << msg.str() << RESET << std::endl;
	send(client->getFd(), msg.str().c_str(), msg.str().size(), MSG_NOSIGNAL);
}

void NumericReplies::sendNumReply(Client *client, int numericCode, const std::string &str1)
{
	if (!client)
		return;

	std::ostringstream msg;
	msg << ":" << Server::getInstance().getServerName() << " " << numericCode << " " << client->getNickname();
	if (!str1.empty())
		msg << " " << str1;
	msg << getErrorMessage(numericCode) << "\r\n";

	std::cout << INVERSE_BG << RED << ">>> " << BOLD << msg.str() << RESET << std::endl;
	send(client->getFd(), msg.str().c_str(), msg.str().size(), MSG_NOSIGNAL);
}

void NumericReplies::sendNumReply(Client *client, int numericCode, const std::string &str1, const std::string &str2)
{
	if (!client)
		return;

	std::ostringstream msg;
	msg << ":" << Server::getInstance().getServerName() << " " << numericCode << " " << client->getNickname();
	if (!str1.empty())
		msg << " " << str1;
	if (!str2.empty())
		msg << " " << str2;
	msg << getErrorMessage(numericCode) << "\r\n";

	std::cout << INVERSE_BG << RED << ">>> " << BOLD << msg.str() << RESET << std::endl;
	send(client->getFd(), msg.str().c_str(), msg.str().size(), MSG_NOSIGNAL);
}
