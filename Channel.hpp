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
		size_t _userLimit;
		bool _inviteOnly;
		bool _topicLocked;
		std::string _topic;
		std::set<Client*> _memberSet;
		std::set<Client*> _operatorSet;
		std::set<Client*> _invitedSet;

	public:
		Channel(Client *client, std::string name, std::string key);
		~Channel();

		// Join command
		void	joinChannel(Client *client, const std::string &key);
		bool	canJoin(Client *client, std::string key);
	
		// Part command
		void	partChannel(Client *client, const std::string &msgPart);

		// Channel search
		static Channel*	findChannel(std::string target);
		std::string	getMembersNick() const;
		std::string	get_name() const;
				
		// Client search
		bool	isInChannel(Client* client) const;
		std::set<int>	getMembersFdSet() const;
		std::set<Client*> const	&get_memberSet() const;

		// Channel key
		std::string	get_key() const;
		void		set_key(const std::string &key);

		// Founder info
		void	set_founderMask(std::string clientMask);

		// User limit
		void	set_userLimit(size_t limit);
		size_t	get_userLimit() const;

		// Invite mode
		void	set_inviteOnly(bool mode);
		bool	get_inviteOnly() const;
		void	addInvite(Client *client);
		bool	isInvited(Client *client) const;

		// Topic lock mode
		void		setTopicLocked(bool mode);
		bool		get_topicLocked() const;
		void		set_topic(const std::string &topic);
		std::string	get_topic() const;

		// Operator management
		void	addOperator(Client* client);
		void	removeOperator(Client* client);
		bool	isOperator(Client* client) const;
};
