#include "Channel.hpp"

Channel::Channel(Client *client, std::string name, std::string key)
	: _name(name),
	_key(key),
	_founderMask(client->getSource()),
	_inviteOnly(false),
	_topicLocked(false),
	_userLimit(0)
{
	_clientsSet.insert(client->getFd());
	_operators.insert(client->getNickname()); // Make creator an operator
}

Channel::~Channel() {}

void Channel::joinChannel(Client *client, std::string channelName, std::string key)
{
	(void)key;
    // if (!canJoin(client, key))
    //     return ;
    this->_clientsSet.insert(client->getFd());
    // Command::sendTopic(client, this);

    std::string msg = client->getSource() + " JOIN " + channelName;

	Reply::sendBroadcast(this->_clientsSet, client, msg);
}

// Find a channel by name (static)
Channel* Channel::findChannel(std::string target)
{
	std::map<std::string, Channel*> _channelMapLocal = Server::getInstance().getChannelMap();
	for (std::map<std::string, Channel*>::iterator it = _channelMapLocal.begin(); it != _channelMapLocal.end(); ++it)
	{
		if (it->first == target)
			return it->second;
	}
	return NULL;
}

std::string Channel::getName() const { return _name; }
std::set<int> Channel::get_clientsSet() const { return _clientsSet; }

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
void Channel::addOperator(Client *client)
{
	if (client)
		_operators.insert(client->getNickname());
}

void Channel::removeOperator(Client *client)
{
	if (client)
		_operators.erase(client->getNickname());
}

bool Channel::isOperator(Client *client) const
{
	return client && _operators.find(client->getNickname()) != _operators.end();
}

// Client presence
bool Channel::isUserInChannel(Client *client) const
{
	return client && _clientsSet.find(client->getFd()) != _clientsSet.end();
}