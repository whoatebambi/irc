#pragma once

#include <string>
#include <sstream>
#include <ctype.h>

class Client;

//////////// Nick message | Numeric Replies:

#define ERR_NONICKNAMEGIVEN   431   // ":No nickname given"
// Returned when a nickname parameter expected for a command
// and isn't found.

#define ERR_ERRONEUSNICKNAME  432   // "<nick> :Erroneous nickname"
// Returned after receiving a NICK message which contains
// characters which do not fall in the defined set.

#define ERR_NICKNAMEINUSE     433   // "<nick> :Nickname is already in use"
// Returned when a NICK message is processed that results
// in an attempt to change to a currently existing nickname.

class Command
{
    public:
		virtual ~Command() {}
		virtual void execute(const std::string &args, Client *client) = 0;

		static void sendMsg(Client *client, std::string msg);
		void	sendRpl(Client *client, int err, std::string msg);
};
