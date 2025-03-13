#pragma once

#include "Server.hpp"
#include "Command.hpp"
#include "CommandCap.hpp"

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

class Client
{
	private:
		int	_fd;
		int	_port;
		std::string	_nickname;
		std::string	_username;
		std::string	_ipadd;
		std::string	_saved;
		std::map<std::string, Command*>	_CommandMap;

	public:
		Client(int fd, std::string ip, int port);
		~Client();

		int getFd() const;
		std::string getNickname() const;
		std::string	getSaved() const;
		
		void	ParseDataClient(int fd);
		void	parse(std::string &line);
		std::string	ft_trim(const std::string &str);
		void	NickCommand(const std::string &args);
		void	UserCommand(const std::string &args);
};
