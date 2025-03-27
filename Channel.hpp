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
        std::set<Client*> _members;
		bool _inviteOnly;
		bool _topicLocked;
		std::string _topic;
		size_t _userLimit;
		std::set<Client*> _operators;
		std::set<Client*> _inviteList;

    public:
        Channel(Client *client, std::string name, std::string key);
        ~Channel();
		void	joinChannel(Client *client, std::string key);
		bool	canJoin(Client *client, std::string key);
        static void    removeFromList(std::set<int> &list, int fd);

		static Channel*	findChannel(std::string target);

		std::string	get_topic() const;
		void set_topic(const std::string &topic);

		std::string	getMembersNick() const;
    	size_t	get_membersCount() const;

		std::string	getFounderMask() const;

    	std::set<Client*> const	&getMembers() const;
		std::set<int>	get_membersFd() const;
		
		std::string	getName() const;

		void setInviteOnly(bool mode);
		bool isInviteOnly() const;

		void setTopicLocked(bool mode);
		bool isTopicLocked() const;

		void setKey(const std::string& key);
		std::string getKey() const;

		void setUserLimit(size_t limit);
		size_t getUserLimit() const;
		void set_userCount(size_t limit);
		size_t get_userCount() const;

		void addOperator(Client* client);
		void removeOperator(Client* client);
		bool	isOperator(Client* client) const;

		bool isInChannel(Client* client) const;
};
