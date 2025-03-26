#pragma once

#include "Client.hpp"
#include "Server.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <map>


//////////// Nick message | Numeric Replies:
#define ERR_NONICKNAMEGIVEN	431	// ":No nickname given"
// Returned when a nickname parameter expected for a command
// and isn't found.
#define ERR_ERRONEUSNICKNAME  432	// "<nick> :Erroneus nickname"
// Returned after receiving a NICK message which contains
// characters which do not fall in the defined set.
#define ERR_NICKNAMEINUSE	433   // "<nick> :Nickname is already in use"
// Returned when a NICK message is processed that results
// in an attempt to change to a currently existing nickname.

//////////// Join message | Numeric Replies:
#define ERR_NEEDMOREPARAMS	461	// "<command> :Not enough parameters"
#define ERR_BANNEDFROMCHAN	474 // "<channel> :Cannot join channel (+b)"                       
#define ERR_INVITEONLYCHAN	473	// "<channel> :Cannot join channel (+i)"
#define ERR_BADCHANNELKEY	475	// "<channel> :Cannot join channel (+k)"                        
#define ERR_CHANNELISFULL	471 // "<channel> :Cannot join channel (+l)"
#define ERR_NOSUCHCHANNEL	403	// "<channel name> :No such channel"
#define ERR_BADCHANMASK		476

#define RPL_UMODEIS			221 // Sent to a client to inform that client of their currently-set user modes.
#define RPL_CHANNELMODEIS	324
#define ERR_CHANOPRIVSNEEDED	482


#define INVERSE "\033[7m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define RESET "\033[0m"
#define INVERSE_BG "\033[7;49m"
#define BOLD "\033[1m"

class Client;

class NumericReplies {
public:
    // Function declarations for sending error messages
	static void	sendNumReply(Client *client, int numericCode);
    static void sendNumReply(Client *client, int numericCode, const std::string &str1);
    static void sendNumReply(Client *client, int numericCode, const std::string &str1, const std::string &str2);
    
    // Initialize the error messages map (called in main or appropriate place)
    static void initErrorMessages();
};


