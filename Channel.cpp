#include "Channel.hpp"

Channel::Channel(Client *client, std::string name, std::string key)
	: _name(name),
	_key(key),
	_founderMask(client->get_mask()),
	_userLimit(0),
	_inviteOnly(false),
	_topicLocked(false)
{
	_memberSet.insert(client);
	_operatorSet.insert(client);
}

Channel::~Channel() {}

// Join command
void	Channel::joinChannel(Client *client, const std::string &key)
{
	if (!canJoin(client, key))
		return;
	this->_memberSet.insert(client);
	Reply::sendBroadcast(getMembersFdSet(), client, " JOIN :" + get_name());
	Reply::sendNumReply(client, RPL_TOPIC, get_topic());
	Reply::sendNumReply(client, RPL_NAMREPLY, "= " + get_name() + getMembersNick());
	Reply::sendNumReply(client, RPL_ENDOFNAMES, get_name());
}

bool	Channel::canJoin(Client *client, std::string key)
{
	if (_inviteOnly && !isInvited(client))
		return (Reply::sendNumReply(client, ERR_INVITEONLYCHAN, get_name()), false);
	if (_userLimit != 0 && _memberSet.size() >= _userLimit)
		return (Reply::sendNumReply(client, ERR_CHANNELISFULL, get_name()), false);
	if (!(_key.empty()) && _key != key)
		return (Reply::sendNumReply(client, ERR_BADCHANNELKEY, get_name()), false);
	return (true);
}

// Part command
void	Channel::partChannel(Client *client, const std::string &msgPart)
{
	if (msgPart == "")
		Reply::sendBroadcast(getMembersFdSet(), client, " PART " + _name);
	else
		Reply::sendBroadcast(getMembersFdSet(), client, " PART " + _name + " :" + msgPart);
	this->_memberSet.erase(client);
	this->_operatorSet.erase(client);
}

// Channel search
Channel*	Channel::findChannel(std::string target)
{
	std::map<std::string, Channel*> _channelMap = Server::getInstance().getChannelMap();
	for (std::map<std::string, Channel*>::iterator it = _channelMap.begin(); it != _channelMap.end(); ++it)
	{
		if (it->first == target)
			return it->second;
	}
	return (NULL);
}

std::string	Channel::getMembersNick() const
{
	std::string str;
	for (std::set<Client*>::const_iterator it = _memberSet.begin(); it != _memberSet.end(); ++it)
	{
		str += " ";
		if (isOperator(*it))
			str += "@";
		str += (*it)->get_nickname();
	}
	return (str);
}

std::string	Channel::get_name() const { return (_name); }

// Client search
bool	Channel::isInChannel(Client *client) const { return client && _memberSet.find(client) != _memberSet.end(); }

std::set<int>	Channel::getMembersFdSet() const
{
	std::set<int> fdSet;
	for (std::set<Client*>::const_iterator it = _memberSet.begin(); it != _memberSet.end(); ++it)
		fdSet.insert((*it)->get_fd());
	return (fdSet);
}

std::set<Client*> const	&Channel::get_memberSet() const { return _memberSet; }

// Channel key
std::string	Channel::get_key() const { return (_key); }
void		Channel::set_key(const std::string &key) { _key = key; }

// Founder info
void	Channel::set_founderMask(std::string clientMask) { _founderMask = clientMask; }

// User limit
void	Channel::set_userLimit(size_t limit) { _userLimit = limit; }
size_t	Channel::get_userLimit() const { return (_userLimit); }

// Invite-only mode
void	Channel::set_inviteOnly(bool mode) { _inviteOnly = mode; }
bool	Channel::get_inviteOnly() const { return (_inviteOnly); }
void	Channel::addInvite(Client *client) { _invitedSet.insert(client); }
bool	Channel::isInvited(Client *client) const { return (_invitedSet.find(client) != _invitedSet.end()); }

// Topic lock mode
void	Channel::setTopicLocked(bool mode) { _topicLocked = mode; }
bool	Channel::get_topicLocked() const { return (_topicLocked); }
void		Channel::set_topic(const std::string &topic) { _topic = topic; }
std::string	Channel::get_topic() const { return (_topic); }

// Operator management
void Channel::addOperator(Client *client) { _operatorSet.insert(client); }
void Channel::removeOperator(Client *client) { _operatorSet.erase(client); }
bool Channel::isOperator(Client* client) const { return (_operatorSet.find(client) != _operatorSet.end()); }
