#pragma once

#include "Client.hpp"

#include <string>
#include <algorithm>
#include <map>
#include <set>
#include <ctime>

class Client;

class Channel
{
    private:
        std::string _name;
        std::string _topic;
        std::string _key;
        std::string _topicEditor;
        std::string _founderMask;
        std::set<int> _clients;

    public:
        Channel(Client *client, std::string name, std::string key);
        ~Channel();
};
