#include "Reply.hpp"

void Reply::sendReply(Client *client, std::string msg)
{
	std::cout << INVERSE_BG << RED << ">>> " << BOLD << msg << RESET << std::endl;
	msg += "\r\n";
	send(client->getFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
}

void Reply::sendBroadcast(std::set<int> fds, const Client* sender, std::string msg)
{
	std::string msgFull = ":" + sender->getSource() + " " + msg + "\r\n";
    for (std::set<int>::const_iterator it = fds.begin(); it != fds.end(); ++it)
    {
        std::cout << INVERSE_BG << RED << ">>> " << BOLD << "SERVER to FD<" << *it << ">" << RESET << msgFull;
        send(*it, msgFull.c_str(), msgFull.size(), MSG_NOSIGNAL);
    }
}


// Error code descriptions
static std::map<int, std::string> replies;

void  Reply::initReplies()
{
	//////////// NICK replies & errors:
	replies.insert(std::make_pair(ERR_NONICKNAMEGIVEN, "No nickname given"));
	replies.insert(std::make_pair(ERR_ERRONEUSNICKNAME, "Erroneus nickname"));
	replies.insert(std::make_pair(ERR_NICKNAMEINUSE, "Nickname is already in use"));
	replies.insert(std::make_pair(ERR_NICKCOLLISION, "Nickname collision KILL"));

	//////////// JOIN replies & errors:
	replies.insert(std::make_pair(ERR_TOOMANYCHANNELS, "You have joined too many channels"));
	replies.insert(std::make_pair(ERR_NEEDMOREPARAMS, "Not enough parameters"));
	replies.insert(std::make_pair(ERR_CHANNELISFULL, "Cannot join channel (+l)"));
	replies.insert(std::make_pair(ERR_INVITEONLYCHAN, "Cannot join channel (+i)"));
	replies.insert(std::make_pair(ERR_BANNEDFROMCHAN, "Cannot join channel (+b)"));
	replies.insert(std::make_pair(ERR_BADCHANNELKEY, "Cannot join channel (+k)"));
	replies.insert(std::make_pair(ERR_BADCHANMASK, "Bad Channel Mask"));
	
	//////////// MODE replies & errors:
	replies.insert(std::make_pair(ERR_NOSUCHNICK, "No such nick/channel"));
	replies.insert(std::make_pair(ERR_NOSUCHCHANNEL, "No such channel"));
	replies.insert(std::make_pair(ERR_NOTONCHANNEL, "You're not on that channel"));
	replies.insert(std::make_pair(ERR_KEYSET, "Channel key already set"));
	replies.insert(std::make_pair(ERR_UNKNOWNMODE, "is unknown mode char to me"));
	replies.insert(std::make_pair(ERR_CHANOPRIVSNEEDED, "You're not channel operator"));
	replies.insert(std::make_pair(ERR_USERSDONTMATCH, "Cant change mode for other users"));
}

// Get error message from map
std::string getErrorMessage(int code)
{
	std::map<int, std::string>::const_iterator it = replies.find(code);
	if (it != replies.end())
		return " :" + it->second;
	return "";
	// return "Unknown Error";
}

void Reply::sendNumReply(Client *client, int numericCode) // sendError
{
	if (!client)
		return;

	std::ostringstream msg;
	msg << ":" << Server::getInstance().getServerName() << " " << numericCode << " " << client->getNickname();
	msg << getErrorMessage(numericCode) << "\r\n";

	std::cout << INVERSE_BG << RED << ">>> " << BOLD << msg.str() << RESET << std::endl;
	send(client->getFd(), msg.str().c_str(), msg.str().size(), MSG_NOSIGNAL);
}

void Reply::sendNumReply(Client *client, int numericCode, const std::string &str1)
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

void Reply::sendNumReply(Client *client, int numericCode, const std::string &str1, const std::string &str2)
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
