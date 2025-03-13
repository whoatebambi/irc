#include "Channel.hpp"

Channel::Channel(Client *client, std::string name, std::string key)
{

    this->_name = name;
    this->_topic = "";
    this->_key = key;
    this->_topicEditor = "";
    _clients.insert(client->getFd());
}

Channel::~Channel()
{
}