#include "CommandJoin.hpp"

void CommandJoin::execute(const std::string &args, Client *client)
{
	std::cout << INVERSE_BG << BLUE << "JOIN command executed with args: " BOLD << args << RESET << std::endl;

	if (args.empty()) // ALREADY MANAGED BY IRSSI | it will not send an empty command
		return (NumericReplies::sendNumReply(client, ERR_NEEDMOREPARAMS, "JOIN"));
		
	std::map<std::string, std::string> argMap = parseArgs(args, client);
	if (argMap.empty())
		return;

	// for (std::map<std::string, std::string>::iterator it = argMap.begin(); it != argMap.end(); ++it)
	// 	std::cout << "Channel: " << it->first << ", Key: " << it->second << std::endl;

	std::map<std::string, Channel*> _channelMapLocal = Server::getInstance().getChannelMap();
	for (std::map<std::string, std::string>::const_iterator it = argMap.begin(); it != argMap.end(); ++it)
	{
        std::map<std::string, Channel*>::const_iterator it2 = _channelMapLocal.begin();
        for (; it2 != _channelMapLocal.end(); ++it2)
        {
            if (sameString(it->first, it2->first))
            {
				std::cout << "SAME STRING CHANNEL MATCH!" << std::endl;
                it2->second->joinChannel(client, it->first, it->second);
                break;
            }
        }
        if (it2 == _channelMapLocal.end())      
			Server::getInstance().newChannel(client, it->first, it->second);
    }   
	// •	Is the channel invite-only? (+i) If so, reject unless the user has an invite.
	// •	Is the channel full? (+l limit) Reject if the user limit is reached.
	// •	Is the channel password-protected? (+k key) Ensure the user provides the correct key.
	// •	Is the user banned? (+b ban list) If so, reject them.

	// •	Add the user to the channel’s member list.
	// •	If this is the first user, they become the operator (+o).
	// •	Update the channel’s user count.

	// •	If the user is already in the channel, ignore the request.
	// •	If the user was kicked and tries to rejoin immediately, enforce a delay (if required).
}

bool sameString(const std::string &s1, const std::string &s2)
{
    if (s1.size() != s2.size())
        return false;
    for (size_t i = 0; i < s1.size(); ++i)
    {
        if (std::tolower(s1[i]) != std::tolower(s2[i]))
            return false;
    }
    return true;
}

std::map<std::string, std::string> CommandJoin::parseArgs(const std::string &args, Client *client)
{
    std::map<std::string, std::string> mapArgs;

    std::vector<std::string> argsVector = splitArgs(args);

    std::vector<std::string> channelVector = splitString(argsVector[0], ',');
	for (size_t i = 0; i < channelVector.size(); ++i)
	{
		if (isValidChannelString(channelVector[i]) == false)
		{
			NumericReplies::sendNumReply(client, ERR_BADCHANMASK, channelVector[i]);
			NumericReplies::sendNumReply(client, ERR_NOSUCHCHANNEL, channelVector[i]); // forces IRSSI behaviour
			return (mapArgs);
		}		
	}
    if (argsVector.size() == 2)
    {
        std::vector<std::string> keys = splitString(argsVector[1], ',');
        if (channelVector.size() != keys.size())
			return (NumericReplies::sendNumReply(client, ERR_NOSUCHCHANNEL, "ERROR"), mapArgs);
		replaceDefaultKey(keys); // Handles IRSSI behaviour that sends key = "x" instead of key = ""
		// !!! need to handle if "x" is added by IRSSI in the channelName column "/join #AAA,,#CCC aaa,bbb,ccc"

        for (size_t i = 0; i < keys.size(); ++i)
        {
            if (!isValidKeyString(keys[i]))
			{
				NumericReplies::sendNumReply(client, ERR_BADCHANMASK, channelVector[i]);
				NumericReplies::sendNumReply(client, ERR_NOSUCHCHANNEL, channelVector[i]); // forces IRSSI behaviour
				return (mapArgs);
			}	
        }
        for (size_t i = 0; i < channelVector.size(); ++i)
            mapArgs[channelVector[i]] = keys[i];
        return mapArgs;
    }
	else
	{
        for (size_t i = 0; i < channelVector.size(); ++i)
            mapArgs[channelVector[i]] = "";
        return mapArgs;
	}
}

// Handles IRSSI behaviour that sends key = "x" instead of key = ""
void replaceDefaultKey(std::vector<std::string> &keys)
{
    for (size_t i = 0; i < keys.size(); ++i) {
        if (keys[i] == "x") {
            keys[i] = "";
        }
    }
}

bool	isValidKeyString(const std::string &key)
{
	// ALREADY MANAGED BY IRSSI | it adds '#' if it is missing
	// Used for replaceDefaultKey()
	if (key.empty())
		return true;
	// Password must be between 3 and 32 characters
	if (key.length() < 3 || key.length() > 32)
	{
		std::cout << "INVALID KEY | key.length() < 3" << std::endl;
		return false;
	}
	for (size_t i = 0; i < key.length(); ++i)
	{
		// Reject control characters (ASCII 0-31) and non-printable characters
		if (key[i] < 32 || !std::isprint(key[i]))
			return false;
		// Reject spaces and commas
		if (key[i] == ' ' || key[i] == ',')
			return false;
	}
	return true;
}

bool	isValidChannelString(const std::string &channel)
{
	// Check if the channel is at least 2 characters long
	if (channel.length() <= 2 || channel.length() >= 50)
		return false;
	// Ensure the channel starts with a # | allow? '&' '+' ...
	if (channel[0] != '#')
		return false;
	// Check for valid characters (alphanumeric, _, -), exclude control characters
	for (size_t i = 1; i < channel.length(); ++i)
	{
		if (!(std::isalnum(channel[i]) || channel[i] == '_' || channel[i] == '-'))
			return false;
	}
	return true;
}
