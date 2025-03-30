#include "CommandCap.hpp"

void CommandCap::execute(const std::string &args, Client *client)
{
	// std::cout << INVERSE_BG << BLUE << "CAP args: " << BOLD << args << RESET << std::endl;
	
	if (args == "LS")
		Reply::sendReply(client, "CAP * LS :");
	else if (args == "END")
		sendWelcome(client);
}

void CommandCap::sendWelcome(Client *client)
{
	if (client->get_nickname().empty() || client->get_username().empty())
		return;

	const std::string	&serverName = Server::getInstance().get_host();
	std::string	nickname = client->get_nickname();
	const std::string &version = "1.0.0";
	const std::string &userModes = "aiwroOs"; // dummy set of user modes
	const std::string &channelModes = "mtov"; // dummy set of channel modes

	std::string msg001 = ":" + serverName + " 001 " + nickname + " :Welcome to the IRC Network " + nickname;
	Reply::sendReply(client, msg001); // RPL_WELCOME 001
	// Reply::sendNumReply(client, RPL_WELCOME, ":Welcome to the IRC Network " + nickname);
	// for this to work, the numericCode need to be string instead of int

	std::string msg002 = ":" + serverName + " 002 " + nickname + " :Your host is " + serverName + ", running version " + version;
	Reply::sendReply(client, msg002); // RPL_YOURHOST 002

	std::string msg003 = ":" + serverName + " 003 " + nickname + " :This server was created <datetime>";
	Reply::sendReply(client, msg003); // RPL_CREATED 003

	std::string msg004 = ":" + serverName + " 004 " + nickname + " " + serverName + " " + version + " " + userModes + " " + channelModes;
	Reply::sendReply(client, msg004); // RPL_MYINFO 004
}
