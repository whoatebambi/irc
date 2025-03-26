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

class Client
{
	private:
		int	_fd;
		int	_port;
		bool _isAuth;
		std::string	_nickname;
		std::string	_username;
		std::string _realname;
		std::string	_ipadd;
		std::string	_saved;
		std::map<std::string, Command*>	_CommandMap; // should be in server?

	public:
		Client(int fd, std::string ip, int port);
		~Client();

		void		ParseDataClient();
		void		parse(std::string &line);
		std::string	ft_trim(const std::string &str);
		bool 		isInList(std::set<int> list);
		
		std::string getSource();
		bool 		getIsAuth() const;
		void 		setIsAuth();
		int			getFd() const;
		std::string	getNickname(); // removed const
		void		setNickname(const std::string &nickname);
		std::string	getSaved() const;
		std::string	getUsername() const;
		void		setUsername(const std::string &username);
		std::string	getRealname() const;
		void		setRealname(const std::string &realname);
};
