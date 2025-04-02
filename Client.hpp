#pragma once

#include "Server.hpp"
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

class Client
{
	private:
		int			_fd;
		std::string	_host;
		std::string	_saved;
		bool 		_isAuth;
		// bool 		_isRegistered; // after PASS, NICK, USER commands, check if useful
		bool		_isDead;
		std::string	_nickname;
		std::string	_username;
		std::string _realname;
		std::string	_mask;

		static const int BUFFER_SIZE = 512;

	public:
		Client(int fd, std::string host);
		~Client();

		void		parseDataClient();
		void		executeCommand(std::string &line);
		std::string	ft_trim(const std::string &str);
		
		int			get_fd() const;
		const std::string	&get_host() const;
		void 		set_isAuth();
		bool 		get_isAuth() const;
		// void		set_isRegistered();
		// bool		get_isRegistered() const;
		void		set_isDead();
		bool		get_isDead() const;
		const std::string	&get_nickname() const;
		void				set_nickname(const std::string &nickname);
		const std::string	&get_username() const;
		void				set_username(const std::string &username);
		const std::string	&get_realname() const;
		void				set_realname(const std::string &realname);
		void				set_mask();
		const std::string &get_mask() const;
};
