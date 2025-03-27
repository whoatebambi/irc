#include "CommandJoin.hpp"

void CommandJoin::execute(const std::string &args, Client *client)
{
	// std::cout << INVERSE_BG << BLUE << "JOIN args: " BOLD << args << RESET << std::endl;

	if (args.empty())
		return (Reply::sendNumReply(client, ERR_NEEDMOREPARAMS, "JOIN"));
	std::map<std::string, std::string> argMap = parseArgs(args, client);
	if (argMap.empty())
		return;

	// for (std::map<std::string, std::string>::iterator it = argMap.begin(); it != argMap.end(); ++it)
	// 	std::cout << "Channel: " << it->first << ", Key: " << it->second << std::endl;
	const std::map<std::string, Channel*> &channelMap = Server::getInstance().getChannelMap();
	for (std::map<std::string, std::string>::const_iterator it = argMap.begin(); it != argMap.end(); ++it)
	{
		const std::string &channelName = it->first;
		const std::string &key = it->second;
        std::map<std::string, Channel*>::const_iterator it2 = channelMap.begin();
        for (; it2 != channelMap.end(); ++it2)
        {
            if (sameString(channelName, it2->first))
            {
				Channel *channel = it2->second;
				if (channel->isInChannel(client))
					return; // client is already in channel
				channel->joinChannel(client, key);
				break;
            }
        }
        if (it2 == channelMap.end())      
			Server::getInstance().newChannel(client, it->first, it->second);
    }
	// •	Add the user to the channel’s member list.
	// •	Update the channel’s user count.
	// •	If this is the first user, they become the operator (+o).
}

std::map<std::string, std::string> CommandJoin::parseArgs(const std::string &args, Client *client)
{
	std::map<std::string, std::string> mapArgs;

	std::vector<std::string> argsVector = splitArgs(args);
	if (argsVector.empty())
		return (mapArgs);

	std::vector<std::string> channelVector = splitString(argsVector[0], ',');
	std::vector<std::string> keyVector;

	if (argsVector.size() == 2)
	{
		keyVector = splitString(argsVector[1], ',');
		replaceDefaultKey(keyVector); // Handle irssi sending key = "x"
	}

	for (size_t i = 0; i < channelVector.size(); ++i)
	{
		const std::string &channel = channelVector[i];
		const std::string key = (i < keyVector.size()) ? keyVector[i] : "";

		if (isValidChannelString(channel) && isValidKeyString(key))
			mapArgs[toLower(channel)] = key;
		else
			Reply::sendNumReply(client, ERR_BADCHANMASK, channel);
	}
	return (mapArgs);
}

std::string toLower(const std::string &str)
{
	std::string result = str;
	for (size_t i = 0; i < result.size(); ++i)
		result[i] = std::tolower(static_cast<unsigned char>(result[i]));
	return (result);
}

bool sameString(const std::string &s1, const std::string &s2)
{
	if (s1.size() != s2.size())
		return (false);
	for (size_t i = 0; i < s1.size(); ++i)
	{
		if (std::tolower(s1[i]) != std::tolower(s2[i]))
			return (false);
	}
	return (true);
}

// Handles IRSSI behaviour that sends key = "x" instead of key = ""
void replaceDefaultKey(std::vector<std::string> &keys)
{
	for (size_t i = 0; i < keys.size(); ++i)
	{
		if (keys[i] == "x") {
			keys[i] = "";
		}
	}
}

bool	isValidKeyString(const std::string &key)
{
	// Used for replaceDefaultKey()
	if (key.empty())
		return (true);

	if (key.length() < 3 || key.length() > 32)
		return (false);

	for (size_t i = 0; i < key.length(); ++i)
	{
		if (!std::isprint(key[i]))  // control characters
			return (false);
		if (key[i] == ' ' || key[i] == ',')
			return (false);
	}
	return (true);
}

bool isValidChannelString(const std::string &channel)
{
	if (channel.empty() || channel.length() < 2|| channel.length() > 50)
		return (false);

	if (channel[0] != '#')
		return (false);

	for (size_t i = 1; i < channel.length(); ++i)
	{
		unsigned char c = channel[i];
		if (c == ' ' || c == ',' || c == '\a' || c == ':')
			return (false);
		if (c <= 0x1F || c == 0x7F) // control characters
			return (false);
	}
	return (true);
}
