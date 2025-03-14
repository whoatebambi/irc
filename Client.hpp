#pragma once

#include "Server.hpp"
#include "Command.hpp"
#include "CommandCap.hpp"
#include "CommandNick.hpp"
#include "CommandUser.hpp"
#include "CommandMode.hpp"
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

class Client
{
	private:
		int	_fd;
		int	_port;
		std::string	_nickname;
		std::string	_username;
		std::string _realname;
		std::string	_ipadd;
		std::string	_saved;
		std::map<std::string, Command*>	_CommandMap;

	public:
		Client(int fd, std::string ip, int port);
		~Client();

		void	ParseDataClient();
		void	parse(std::string &line);
		std::string	ft_trim(const std::string &str);
		std::string getSource();

		int			getFd() const;
		std::string	getNickname(); // removed const
		void		setNickname(const std::string &nickname);
		std::string	getSaved() const;
		std::string	getUsername() const;
		void		setUsername(const std::string &username);
		std::string	getRealname() const;
		void		setRealname(const std::string &realname);
};
