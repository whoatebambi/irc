#include "Reply.hpp"

void Reply::sendReply(Client *client, std::string msg)
{
	std::cout << INVERSE_BG << RED << ">>> " << BOLD << msg << RESET << std::endl;
	msg += "\r\n";
	send(client->get_fd(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
}

void Reply::sendBroadcast(std::set<int> fds, Client* sender, std::string msg)
{
	std::string msgFull = sender->get_mask() + " " + msg + "\r\n";
	for (std::set<int>::const_iterator it = fds.begin(); it != fds.end(); ++it)
	{
		std::cout << INVERSE_BG << RED << ">>> " << BOLD << "server->FD<" << *it << "> " << msgFull << RESET ;
		send(*it, msgFull.c_str(), msgFull.size(), MSG_NOSIGNAL);
	}
}


// Error code descriptions
static std::map<int, std::string> replies;

void  Reply::initReplies()
{
	//////////// PASS replies & errors:
	replies.insert(std::make_pair(ERR_NEEDMOREPARAMS, "Not enough parameters"));
	replies.insert(std::make_pair(ERR_ALREADYREGISTRED, "You may not reregister"));

	//////////// NICK replies & errors:
	replies.insert(std::make_pair(ERR_NONICKNAMEGIVEN, "No nickname given"));
	replies.insert(std::make_pair(ERR_ERRONEUSNICKNAME, "Erroneus nickname"));
	replies.insert(std::make_pair(ERR_NICKNAMEINUSE, "Nickname is already in use"));

	//////////// JOIN replies & errors:
	replies.insert(std::make_pair(RPL_ENDOFNAMES, "End of /NAMES list"));
	replies.insert(std::make_pair(RPL_ENDOFBANLIST, "End of channel ban list"));
	replies.insert(std::make_pair(ERR_CHANNELISFULL, "Cannot join channel (+l)"));
	replies.insert(std::make_pair(ERR_INVITEONLYCHAN, "Cannot join channel (+i)"));
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

	//////////// PRIVMSG replies & errors:
	replies.insert(std::make_pair(ERR_CANNOTSENDTOCHAN, "Cannot send to channel"));
	replies.insert(std::make_pair(ERR_TOOMANYTARGETS, "Duplicate recipients. No message delivered."));
	replies.insert(std::make_pair(ERR_NORECIPIENT, "No recipient given ")); // ":No recipient given (<command>)"
	replies.insert(std::make_pair(ERR_NOTEXTTOSEND, "No text to send"));

	//////////// TOPIC replies & errors:
	replies.insert(std::make_pair(RPL_NOTOPIC, "No topic is set"));
	replies.insert(std::make_pair(RPL_TOPIC, " ")); // test it
	replies.insert(std::make_pair(ERR_INVALIDTOPIC, "Topic is invalid"));

	//////////// INVITE replies & errors:
	replies.insert(std::make_pair(ERR_USERONCHANNEL, "is already on channel"));

	//////////// KICK replies & errors:
	replies.insert(std::make_pair(ERR_USERNOTINCHANNEL, "They aren't on that channel"));
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
	msg << ":" << Server::getInstance().getServerName() << " " << numericCode << " " << client->get_nickname();
	msg << getErrorMessage(numericCode) << "\r\n";

	std::cout << INVERSE_BG << RED << ">>> " << BOLD << msg.str() << RESET << std::endl;
	send(client->get_fd(), msg.str().c_str(), msg.str().size(), MSG_NOSIGNAL);
}

void Reply::sendNumReply(Client *client, int numericCode, const std::string &str1)
{
	if (!client)
		return;

	std::ostringstream msg;
	msg << ":" << Server::getInstance().getServerName() << " " << numericCode << " " << client->get_nickname();
	if (!str1.empty())
		msg << " " << str1;
	msg << getErrorMessage(numericCode) << "\r\n";

	std::cout << INVERSE_BG << RED << ">>> " << BOLD << msg.str() << RESET << std::endl;
	send(client->get_fd(), msg.str().c_str(), msg.str().size(), MSG_NOSIGNAL);
}

void Reply::sendNumReply(Client *client, int numericCode, const std::string &str1, const std::string &str2)
{
	if (!client)
		return;

	std::ostringstream msg;
	msg << ":" << Server::getInstance().getServerName() << " " << numericCode << " " << client->get_nickname();
	if (!str1.empty())
		msg << " " << str1;
	if (!str2.empty())
		msg << " " << str2;
	msg << getErrorMessage(numericCode) << "\r\n";

	std::cout << INVERSE_BG << RED << ">>> " << BOLD << msg.str() << RESET << std::endl;
	send(client->get_fd(), msg.str().c_str(), msg.str().size(), MSG_NOSIGNAL);
}
