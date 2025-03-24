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
        std::string	_name;
        std::string	_key;
        std::string	_founderMask;
        std::set<int>	_clientsSet;

    public:
        Channel(Client *client, std::string name, std::string key);
        ~Channel();
		void	joinChannel(Client *client, std::string channelName, std::string key);
        static void    removeFromList(std::set<int> &list, int fd);

		void		setFounderMask(const std::string &founderMask);
		std::string	getFounderMask() const;
        std::set<int> &getMembers();
};
