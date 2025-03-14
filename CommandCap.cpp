#include "CommandCap.hpp"

void CommandCap::execute(const std::string &args, Client *client)
{
	std::cout << INVERSE_BG << BLUE << "CAP command executed with args: " << BOLD << args << RESET << std::endl;
	
	if (args == "LS")
		sendMsg(client, "CAP * LS :");
	else if (args == "END")
	{
		sendWelcome(client);
		// sendMsg(client, "CAP * END");
	}
}

void CommandCap::sendWelcome(Client *client)
{
	if (client->getNickname().empty() || client->getUsername().empty())
		return;

	std::string nick = client->getNickname();

	// std::cout << "Server name: " << Server::getInstance().getServerName() << std::endl;

	std::string	serverName = Server::getInstance().getServerName();
	std::string	nickname = client->getNickname();

	std::string welcomeMessage = ":" + serverName + " 001 " + nickname + " :Welcome to the IRC Network " + nickname;
    sendMsg(client, welcomeMessage);	// sendMsg(client, "001 " + nick + " :Welcome to the IRC server, " + nick); // "<client> :Welcome to the <networkname> Network, <nick>[!<user>@<host>]"
	// sendMsg(client, "002 " + nick + " :Your host is server-name");
	// sendMsg(client, "003 " + nick + " :This server was created [date-of-creation]");
	// sendMsg(client, "004 " + nick + " " + "CHANNEL 01" + " 1.0"); // -> list available channel / user modes
	// sendMsg(client, "005 " + nick);
	// sendMsg(client, "251 " + nick);
	// sendMsg(client, "252 " + nick);
	// sendMsg(client, "253 " + nick);
	// sendMsg(client, "254 " + nick);
	// sendMsg(client, "255 " + nick);
	// sendMsg(client, "265 " + nick);
	// sendMsg(client, "266 " + nick);
	// sendMsg(client, "375 " + nick + " :- MESSAGE OF THE DAY -");
	// sendMsg(client, "372 " + nick + " :- AD ASTRAM -");
	// sendMsg(client, "376 " + nick + " :- PEACE -");
	// sendMsg(client, "251 " + nick);
}
