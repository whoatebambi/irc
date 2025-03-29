#include "CommandTopic.hpp"

void CommandTopic::execute(const std::string &args, Client *client)
{
	std::cout << INVERSE_BG << BLUE << "TOPIC args: " BOLD << args << RESET << std::endl;

	std::vector<std::string> argVector = splitArgs(args);
	for (size_t i = 0; i < argVector.size(); ++i)
	{ std::cout << "argVector[" << i << "]: " << argVector[i] << std::endl;}

	if (argVector.size() < 1 || argVector.size() > 2)
		return (Reply::sendNumReply(client, ERR_NEEDMOREPARAMS, "TOPIC"));
	const std::string &target = argVector[0];
	if (target[0] != '#')
		return (Reply::sendNumReply(client, ERR_NOSUCHCHANNEL, target));
	Channel *channel = Channel::findChannel(target);
	if (!channel)
		return (Reply::sendNumReply(client, ERR_NOSUCHCHANNEL, target));

	if (!channel->isInChannel(client))
		return (Reply::sendNumReply(client, ERR_NOTONCHANNEL, target));
	
	if (argVector.size() == 1)
	{
		if (channel->get_topic().empty())
			return (Reply::sendNumReply(client, RPL_NOTOPIC, target));
		return (Reply::sendNumReply(client, RPL_TOPIC, target, channel->get_topic()));
	}

	if(!channel->isOperator(client) && channel->get_topicLocked())
		return (Reply::sendNumReply(client, ERR_CHANOPRIVSNEEDED, target));
	
	if (!isValidChannelTopic(argVector[1]))
		return (Reply::sendNumReply(client, ERR_NEEDMOREPARAMS, "TOPIC")); // custom
	
	// remove
	if (argVector[1] == ":")
	{
		channel->set_topic("");
		Reply::sendBroadcast(channel->getMembersFdSet(), client, "TOPIC " + channel->get_name() + " :");
		return;
	}
	//set	
	channel->set_topic(argVector[1]);
	Reply::sendBroadcast(channel->getMembersFdSet(), client, "TOPIC " + channel->get_name() + " :" + argVector[1]);
}

bool isValidChannelTopic(const std::string &topic)
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
		if (c <= 0x1F || c == 0x7F) // control characters
			return (false);
	}
	return (true);
}