#include "Channel.hpp"

Channel::Channel(Client *client, std::string name, std::string key)
{
    this->_name = name;
    this->_key = key;
    _clientsSet.insert(client->getFd());
}

Channel::~Channel() {}

void Channel::setFounderMask(const std::string &founderMask)
{
	this->_founderMask = founderMask;
}

std::string Channel::getFounderMask() const
{
	return this->_founderMask;
}

void Channel::joinChannel(Client *client, std::string channelName, std::string key)
{
	(void)key;
    // if (!canJoin(client, key))
    //     return ;
    this->_clientsSet.insert(client->getFd());
    // Command::sendTopic(client, this);
    
    std::string msg = client->getSource() + " JOIN " + channelName;

	CommandJoin commandJoin;
    commandJoin.broadcast(this->_clientsSet, msg);
}

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
