#pragma once

#include "Client.hpp"
#include "Server.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <set>

//////////// CAP replies & errors:
#define RPL_WELCOME				001 // ✅ "<client> :Welcome to the <networkname> Network, <nick>[!<user>@<host>]"
#define RPL_YOURHOST			002 // ✅ "<client> :Your host is <servername>, running version <version>"
#define RPL_CREATED				003 // "<client> :This server was created <datetime>"
#define RPL_MYINFO				004 // ✅ "<client> <servername> <version> <available user modes>

//////////// PASS replies & errors:
#define ERR_NEEDMOREPARAMS		461	// ✅ "<command> :Not enough parameters"
#define ERR_ALREADYREGISTRED	462 // ✅ :You may not reregister"

//////////// NICK replies & errors:
#define ERR_NONICKNAMEGIVEN		431	// ✅ ":No nickname given"
#define ERR_ERRONEUSNICKNAME	432	// ✅ "<nick> :Erroneus nickname"
#define ERR_NICKNAMEINUSE		433 // ✅ "<nick> :Nickname is already in use"

//////////// USER replies & errors:
#define ERR_NEEDMOREPARAMS		461	// ✅ "<command> :Not enough parameters"
#define ERR_ALREADYREGISTRED	462 // ✅ :You may not reregister"

//////////// JOIN replies & errors:
#define RPL_TOPIC				332 // ✅ "<channel> :<topic>"
#define RPL_NAMREPLY			353 // ✅ "<channel> :[[@|+]<nick> [[@|+]<nick> [...]]]"
#define RPL_ENDOFNAMES			366 // ✅ "<channel> :End of /NAMES list"
#define RPL_ENDOFBANLIST		368 // ✅ End of channel ban list
#define ERR_NEEDMOREPARAMS		461	// ✅ "<command> :Not enough parameters"
#define ERR_CHANNELISFULL		471 // ✅ "<channel> :Cannot join channel (+l)"
#define ERR_INVITEONLYCHAN		473	// ✅ "<channel> :Cannot join channel (+i)"                     
#define ERR_BADCHANNELKEY		475	// ✅ "<channel> :Cannot join channel (+k)"                        
#define ERR_BADCHANMASK			476 // ✅ "<channel> :Bad channel mask"

//////////// PART replies & errors:
#define ERR_NOSUCHCHANNEL		403	// "<channel name> :No such channel"
#define ERR_NOTONCHANNEL		442 // "<channel> :You're not on that channel"
#define ERR_NEEDMOREPARAMS		461	// "<command> :Not enough parameters"

//////////// TOPIC replies & errors:
// CHANNEL OPERATORS ONLY:
#define RPL_NOTOPIC				331 // ✅ "<channel> :No topic is set"
#define RPL_TOPIC				332 // ✅ "<channel> :<topic>"
#define ERR_NOTONCHANNEL		442 // ✅ "<channel> :You're not on that channel"
#define ERR_NEEDMOREPARAMS		461	// ✅ "<command> :Not enough parameters"
#define ERR_CHANOPRIVSNEEDED	482 // ✅ "<channel> :You're not channel operator"

//////////// INVITE replies & errors:
// CHANNEL OPERATORS ONLY: Invite a client to an invite-only channel (mode +i)
#define RPL_AWAY				301 // "<nick> :<away message>"
#define RPL_INVITING			341 // "<channel> <nick>"
#define ERR_NOSUCHNICK			401 // "<nickname> :No such nick/channel"
#define ERR_NOTONCHANNEL		442 // "<channel> :You're not on that channel"
#define ERR_USERONCHANNEL		443 // "<user> <channel> :is already on channel"
#define ERR_NEEDMOREPARAMS		461	// "<command> :Not enough parameters"
#define ERR_CHANOPRIVSNEEDED	482 // "<channel> :You're not channel operator"

//////////// KICK replies & errors:
// CHANNEL OPERATORS ONLY: Eject a client from the channel
#define ERR_NOSUCHCHANNEL		403	// "<channel name> :No such channel"
#define ERR_NOTONCHANNEL		442 // "<channel> :You're not on that channel"
#define ERR_NEEDMOREPARAMS		461	// "<command> :Not enough parameters"
#define ERR_BADCHANMASK			476 // "<channel> :Bad channel mask"
#define ERR_CHANOPRIVSNEEDED	482 // "<channel> :You're not channel operator"

//////////// MODE replies & errors:
// CHANNEL OPERATORS ONLY: Change the channel's mode
#define RPL_UMODEIS				221 // ✅ ":<user mode string>"
#define RPL_CHANNELMODEIS		324 // ✅ "<channel> <mode> <mode params>"
#define ERR_NOSUCHNICK			401 // ✅ "<nickname> :No such nick/channel"
#define ERR_NOSUCHCHANNEL		403	// ✅ "<channel name> :No such channel"
#define ERR_NOTONCHANNEL		442 // ✅ "<channel> :You're not on that channel"
#define ERR_NEEDMOREPARAMS		461	// ✅ "<command> :Not enough parameters"
#define ERR_KEYSET				467 // ✅ "<channel> :Channel key already set"
#define ERR_UNKNOWNMODE			472 // ✅ "<char> :is unknown mode char to me"
#define ERR_CHANOPRIVSNEEDED	482 // ✅ "<channel> :You're not channel operator"
#define ERR_USERSDONTMATCH		502 // ✅ ":Cant change mode for other users"

//////////// PRIVMSG replies & errors:
#define RPL_AWAY				301 // ✅ "<nick> :<away message>"
#define ERR_NOSUCHNICK			401 // ✅ "<nickname> :No such nick/channel"
#define ERR_NOSUCHCHANNEL		403	// ✅ "<channel name> :No such channel"
#define ERR_CANNOTSENDTOCHAN	404 // "<channel name> :Cannot send to channel"
#define ERR_TOOMANYTARGETS		407 // "<target> :Duplicate recipients. No message delivered."
#define ERR_NORECIPIENT			411 // ":No recipient given (<command>)"
#define ERR_NOTEXTTOSEND		412 // ":No text to send"
#define ERR_NEEDMOREPARAMS		461	// "<command> :Not enough parameters"

//////////// QUIT replies & errors:
// none

// optional: USERS to show users, NAMES to show @ ops, LIST channels and topics, OPER


#define INVERSE "\033[7m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define RESET "\033[0m"
#define INVERSE_BG "\033[7;49m"
#define BOLD "\033[1m"


class Client;

class Reply {
	public:
		static void	sendReply(Client *client, std::string msg);
		static void	sendBroadcast(std::set<int> fds, Client* sender, std::string msg);

		static void	sendNumReply(Client *client, int numericCode);
		static void sendNumReply(Client *client, int numericCode, const std::string &str1);
		static void sendNumReply(Client *client, int numericCode, const std::string &str1, const std::string &str2);
		
		static void initReplies(); // find where is best to place it
};
