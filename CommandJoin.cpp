#include "CommandJoin.hpp"

void CommandJoin::execute(const std::string &args, Client *client)
{
	// std::cout << INVERSE_BG << BLUE << "JOIN args: " BOLD << args << RESET << std::endl;

	if (args.empty())
		return (Reply::sendNumReply(client, ERR_NEEDMOREPARAMS, "JOIN"));
	std::map<std::string, std::string> argMap = parseArgs(args, client);
	if (argMap.empty())
		return;

	const std::set<Channel*> &channelSet = Server::getInstance().get_channelSet();
	for (std::map<std::string, std::string>::const_iterator it = argMap.begin(); it != argMap.end(); ++it)
	{
		const std::string &channelName = it->first;
		const std::string &key = it->second;
        std::set<Channel*>::const_iterator it2 = channelSet.begin();
        for (; it2 != channelSet.end(); ++it2)
        {
            if (sameString(channelName, (*it2)->get_name()))
            {
				Channel *channel = *it2;
				if (channel->isInChannel(client))
					return; // client is already in channel
				channel->joinChannel(client, key);
				break;
            }
        }
        if (it2 == channelSet.end())      
			Server::getInstance().addChannel(client, it->first, it->second);
    }
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
