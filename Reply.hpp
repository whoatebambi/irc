#pragma once

#include "Client.hpp"
#include "Server.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <set>


//////////// NICK replies & errors:
#define ERR_NONICKNAMEGIVEN		431	// ":No nickname given"
#define ERR_ERRONEUSNICKNAME	432	// "<nick> :Erroneus nickname"
#define ERR_NICKNAMEINUSE		433 // "<nick> :Nickname is already in use"
#define ERR_NICKCOLLISION		436 // "<nick> :Nickname collision KILL"


//////////// JOIN replies & errors:
#define RPL_NAMREPLY			353 // "<channel> :[[@|+]<nick> [[@|+]<nick> [...]]]"
#define RPL_TOPIC				332 // "<channel> :<topic>"
#define ERR_TOOMANYCHANNELS		405 // "<channel name> :You have joined too many channels"
#define ERR_NEEDMOREPARAMS		461	// ✅ "<command> :Not enough parameters"
#define ERR_CHANNELISFULL		471 // "<channel> :Cannot join channel (+l)"
#define ERR_INVITEONLYCHAN		473	// "<channel> :Cannot join channel (+i)"
#define ERR_BANNEDFROMCHAN		474 // "<channel> :Cannot join channel (+b)"                       
#define ERR_BADCHANNELKEY		475	// "<channel> :Cannot join channel (+k)"                        
#define ERR_BADCHANMASK			476 // ✅ "<channel> :Bad channel mask"


//////////// MODE replies & errors:
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
		static void	sendBroadcast(std::set<int> fds, const Client* sender, std::string msg);

		static void	sendNumReply(Client *client, int numericCode);
		static void sendNumReply(Client *client, int numericCode, const std::string &str1);
		static void sendNumReply(Client *client, int numericCode, const std::string &str1, const std::string &str2);
		
		static void initReplies(); // find where is best to place it
};
