#include "Channel.hpp"

Channel::Channel(Client *client, std::string name, std::string key)
	: _name(name),
	_key(key),
	_founderMask(client->get_mask()),
	_inviteOnly(false),
	_topicLocked(false),
	_userLimit(0)
{
	_members.insert(client);
	_operators.insert(client); // make creator an operator
}

Channel::~Channel() {}

std::set<Client*> const &Channel::getMembers() const { return _members; }

std::set<int> Channel::get_membersFd() const
{
	std::set<int> fds;
	for (std::set<Client*>::const_iterator it = _members.begin(); it != _members.end(); ++it)
		fds.insert((*it)->getFd());
	return fds;
}

bool Channel::canJoin(Client *client, std::string key)
{
	if (_inviteOnly)// && !isInvited(client))
		return (Reply::sendNumReply(client, ERR_INVITEONLYCHAN, getName()), false);

	if (_userLimit != 0 && get_membersCount() >= _userLimit)
		return (Reply::sendNumReply(client, ERR_CHANNELISFULL, getName()), false);

	if (!(_key.empty()) && _key != key)
		return (Reply::sendNumReply(client, ERR_BADCHANNELKEY, getName()), false);

	return (true);
}

void Channel::joinChannel(Client *client, std::string key)
{
    if (!canJoin(client, key))
        return ;
    this->_members.insert(client);

	Reply::sendBroadcast(get_membersFd(), client, " JOIN :" + getName());
	
	return (Reply::sendNumReply(client, RPL_TOPIC, get_topic()));

	Reply::sendNumReply(client, RPL_NAMREPLY, "= " + getName() + getMembersNick());
	Reply::sendNumReply(client, RPL_ENDOFNAMES, getName());
}

std::string	Channel::get_topic() const { return _topic; }
void	Channel::set_topic(const std::string &topic) { _topic = topic; }

std::string Channel::getMembersNick() const
{
	std::string str;
	for (std::set<Client*>::const_iterator it = _members.begin(); it != _members.end(); ++it)
	{
		str += " ";
		if (isOperator(*it))
			str += "@";
		str += (*it)->get_nickname();
	}
	// std::cout << "DEBUG getMembersNick() = " << str << std::endl;
	return (str);
}

void Channel::removeFromList(std::set<int> &list, int fd) { list.erase(fd);}

Channel* Channel::findChannel(std::string target)
{
	std::map<std::string, Channel*> _channelMapLocal = Server::getInstance().getChannelMap();
	for (std::map<std::string, Channel*>::iterator it = _channelMapLocal.begin(); it != _channelMapLocal.end(); ++it)
	{
		if (it->first == target)
			return it->second;
	}
	return (NULL);
}

size_t Channel::get_membersCount() const { return _members.size(); }

std::string Channel::getName() const { return _name; }

// Founder info
std::string Channel::getFounderMask() const { return _founderMask; }

// Invite-only mode
void Channel::setInviteOnly(bool mode) { _inviteOnly = mode; }
bool Channel::isInviteOnly() const { return _inviteOnly; }

// Topic lock mode
void Channel::setTopicLocked(bool mode) { _topicLocked = mode; }
bool Channel::isTopicLocked() const { return _topicLocked; }

// Channel key
void Channel::setKey(const std::string &key) { _key = key; }
std::string Channel::getKey() const { return _key; }

// User limit
void Channel::setUserLimit(size_t limit) { _userLimit = limit; }
size_t Channel::getUserLimit() const { return _userLimit; }

// Operator management
void Channel::addOperator(Client *client) { _operators.insert(client); }
void Channel::removeOperator(Client *client) { _operators.erase(client); }
bool Channel::isOperator(Client* client) const { return (_operators.find(client) != _operators.end()); }

// Client presence
bool Channel::isInChannel(Client *client) const { return client && _members.find(client) != _members.end(); }
