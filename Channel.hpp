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
		bool _inviteOnly;
		bool _topicLocked;
		size_t _userLimit;
		std::set<std::string> _operators;

    public:
        Channel(Client *client, std::string name, std::string key);
        ~Channel();
		void	joinChannel(Client *client, std::string channelName, std::string key);

		static Channel*	findChannel(std::string target);

		std::string	getName() const;

		std::set<int>	get_clientsSet() const;

		std::string	getFounderMask() const;

		void setInviteOnly(bool mode);
		bool isInviteOnly() const;

		void setTopicLocked(bool mode);
		bool isTopicLocked() const;

		void setKey(const std::string& key);
		std::string getKey() const;

		void setUserLimit(size_t limit);
		size_t getUserLimit() const;

		void addOperator(Client* client);
		void removeOperator(Client* client);
		bool isOperator(Client* client) const;

		bool isUserInChannel(Client* client) const;
};
