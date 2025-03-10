#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <set>
#include <map>

#include "ErrMsg.hpp"

class CliSocket;
class Channel;

#define RPL_UMODEIS           221   // "<client> <user modes>" always empty in our case
#define RPL_CHANNELMODEIS     324   // "<client> <channel> <modestring> <mode arguments>..."
#define RPL_CREATIONTIME      329   // "<client> <channel> <creationtime>"
#define RPL_INVITING          341   // "<client> <nick> <channel>"
#define RPL_NAMREPLY          353   // "<client> <symbol> <channel> :[prefix]<nick>{ [prefix]<nick>}"
#define RPL_ENDOFNAMES        366   // "<client> <channel> :End of /NAMES list"
#define RPL_BANLIST           367   // "<client> <channel> <mask> [<who> <set-ts>]"
#define RPL_ENDOFBANLIST      368   // "<client> <channel> :End of channel ban list"



#define ERR_NOSUCHNICK        401   // "<client> <nick> :No such nick/channel"
#define ERR_NOSUCHCHANNEL     403   // "<client> <channel> :No such channel"
#define ERR_CANOTSENDTOCHAN   404   // "<client> <channel> :Cannot send to channel"
#define ERR_NORECIPIENT       411   // "<client> :No recipient given"
#define ERR_NOTEXTTOSEND      412   // "<client> :No text to send"
#define ERR_NONICKNAMEGIVEN   431   // "<client> :No nickname given"
#define ERR_ERRONEUSNICKNAME  432   // "<client> <nick> :Erroneous nickname"
#define ERR_NICKNAMEINUSE     433   // "<client> <nick> :Nickname is already in use"
#define ERR_USERNOTINCHANNEL  441   // "<client> <nick> <channel> :They aren't on that channel"
#define ERR_NOTONCHANNEL      442   // "<client> <channel> :You're not on that channel"
#define ERR_USERONCHANNEL     443   // "<client> <nick> <channel> :is already on channel"
#define ERR_NOTREGISTERED     451   // "<client> :You have not registered"
#define ERR_NEEDMOREPARAMS    461   // "<client> <command> :Not enough parameters"
#define ERR_ALREADYREGISTERED 462   // "<client> :You may not reregister"
#define ERR_PASSWDMISMATCH    464   // "<client> :Password incorrect"
#define ERR_CHANNELISFULL     471   // "<client> <channel> :Cannot join channel (+l)"
#define ERR_UNKNOWNMODE       472   // "<client> <modechar> :is unknown mode char to me""
#define ERR_INVITEONLYCHAN    473   // "<client> <channel> :Cannot join channel (+i)"
#define ERR_BANNEDFROMCHAN    474   // "<client> <channel> :Cannot join channel (+b)"
#define ERR_BADCHANNELKEY     475   // "<client> <channel> :Cannot join channel (+k)"
#define ERR_BADCHANMASK       476   // "<channel> :Bad Channel Mask"
#define ERR_CHANOPRIVSNEEDED  482   // "<client> <channel> :You're not channel operator"
#define ERR_USERSDONTMATCH    502   // "<client> :Cant change mode for other users"
#define ERR_INVALIDKEY        525   // "<client> <target chan> :Key is not well-formed"


class Command
{
    private:
        static ErrMsg errMsg;

    public:
        virtual ~Command() = 0;
        virtual void execute(const std::string &args, CliSocket *client) = 0;


        
        static void sendMsg(CliSocket *client, std::string msg);
        void sendWelcome(CliSocket *client);
        //void sendMotd(CliSocket *client);
        static void sendRpl(CliSocket *client, int err, ...);

        static void sendTopic(CliSocket *client, Channel *channel);

        static void broadcast(std::set<int> fds, std::string msg);


        //parsing
        
        static std::vector<std::string> splitArgs(const std::string &input);
        static std::vector<std::string> splitString(const std::string &str, char delimiter);

        bool isChanOp(Channel *chan, CliSocket *client);
        static CliSocket *findUserFd(std::string user);
        static Channel *chanExist(std::string chanName);
    
        static CliSocket *findClientFromNick(std::string nick);

};

