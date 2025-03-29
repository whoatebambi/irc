#include "CommandTopic.hpp"

static void	replyCurrentTopic(Channel *channel, Client *client);
static bool	canChangeTopic(Channel *channel, Client *client);
static void	broadcastTopicChange(Channel *channel, const std::string &topic, Client *client);
static bool	isValidChannelTopic(const std::string &topic);

void CommandTopic::execute(const std::string &args, Client *client)
{
	// std::cout << INVERSE_BG << BLUE << "TOPIC args: " BOLD << args << RESET << std::endl;
	std::vector<std::string> argVector = splitArgs(args);
	if (argVector.empty() || argVector.size() > 2)
		return (Reply::sendNumReply(client, ERR_NEEDMOREPARAMS, "TOPIC"));

	const std::string &target = argVector[0];
	Channel *channel = Channel::findChannel(target);
	if (!channel)
		return (Reply::sendNumReply(client, ERR_NOSUCHCHANNEL, target));
	if (!channel->isInChannel(client))
		return (Reply::sendNumReply(client, ERR_NOTONCHANNEL, target));

	// No topic argument = request current topic
	if (argVector.size() == 1)
		return (replyCurrentTopic(channel, client));

	// Check privileges and content
	if (!canChangeTopic(channel, client))
		return (Reply::sendNumReply(client, ERR_CHANOPRIVSNEEDED, target));
	
	const std::string &newTopic = argVector[1];
	if (!isValidChannelTopic(newTopic))
		return (Reply::sendNumReply(client, ERR_INVALIDTOPIC, target));

	// Set new topic or erase topic
	if (newTopic == ":")
		return (broadcastTopicChange(channel, "", client));
	broadcastTopicChange(channel, newTopic, client);
}

static void	broadcastTopicChange(Channel *channel, const std::string &topic, Client *client)
{
	channel->set_topic(topic);
	Reply::sendBroadcast(channel->generateMembersFd(), client, "TOPIC " + channel->get_name() + " :" + topic);
}

static void	replyCurrentTopic(Channel *channel, Client *client)
{
	if (channel->get_topic().empty())
		Reply::sendNumReply(client, RPL_NOTOPIC, channel->get_name());
	else
		Reply::sendNumReply(client, RPL_TOPIC, channel->get_name(), channel->get_topic());
}

static bool	canChangeTopic(Channel *channel, Client *client)
{
	if (!channel->get_topicLocked())
		return (true);
	return (channel->isOperator(client));
}

static bool	isValidChannelTopic(const std::string &topic)
{
	if (topic.empty())
		return (true);

	if (topic.length() > 50)
		return (false);

	for (size_t i = 1; i < topic.length(); ++i)
	{
		unsigned char c = topic[i];
		if (c == '\r' || c == '\n' || c == '\t' || c == '\0')
			return (false);
		if (c <= 0x1F || c == 0x7F)
			return (false);
	}
	return (true);
}
