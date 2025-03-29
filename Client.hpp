#pragma once

#include "Server.hpp"
#include "Command.hpp"
#include "CommandCap.hpp"
#include "CommandNick.hpp"
#include "CommandUser.hpp"
#include "CommandMode.hpp"
#include "CommandJoin.hpp"
#include "CommandPass.hpp"
#include "CommandPing.hpp"
#include "CommandPrivMsg.hpp"
#include "CommandTopic.hpp"
#include "CommandPart.hpp"
#include "CommandInvite.hpp"
#include "Channel.hpp"

#include <cstdarg>
#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <map>
#include <set>
#include <sstream>
#include <functional>

class Command;
class CommandCap;
class CommandNick;
class CommandUser;
class CommandMode;
class CommandJoin;
class CommandPass;
class CommandPing;
class CommandTopic;
class CommandPart;
class CommandInvite;

class Client
{
	private:
		int			_fd;
		std::string	_ipadd;
		int			_port;
		std::string	_saved;
		bool 		_isAuth;
		bool 		_isRegistered; // after PASS, NICK, USER commands
		std::string	_nickname;
		std::string	_username;
		std::string _realname;
		std::string	_mask;
		std::map<std::string, Command*>	_CommandMap; // should be in server?

	public:
		Client(int fd, std::string ip, int port);
		~Client();

		void		parseDataClient();
		void		executeCommand(std::string &line);
		std::string	ft_trim(const std::string &str);

		bool	isInList(const std::set<Client*> &list) const ;
		
		void		set_mask();
		std::string get_mask() const;
		void 		set_isAuth();
		bool 		get_isAuth() const;
		void		set_isRegistered();
		bool		get_isRegistered() const;
		int			get_fd() const;
		std::string	get_nickname() const;
		void		set_nickname(const std::string &nickname);
		std::string	get_saved() const;
		std::string	get_username() const;
		void		set_username(const std::string &username);
		std::string	get_realname() const;
		void		set_realname(const std::string &realname);
};
