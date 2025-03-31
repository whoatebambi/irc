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
		std::string	_hostname;
		int			_port;
		std::string	_saved;
		bool 		_isAuth;
		bool 		_isRegistered; // after PASS, NICK, USER commands
		std::string	_nickname;
		std::string	_username;
		std::string _realname;
		std::string	_mask;

	public:
		Client(int fd, std::string hostname, int port);
		~Client();

		void		parseDataClient();
		void		executeCommand(std::string &line);
		std::string	ft_trim(const std::string &str);

		bool	isInList(const std::set<Client*> &list) const; // remove if possible
		
		int			get_fd() const;
		const std::string	&get_hostname() const;
		std::string	get_saved() const;
		void 		set_isAuth();
		bool 		get_isAuth() const;
		void		set_isRegistered();
		bool		get_isRegistered() const;
		std::string get_source() const;
		std::string	get_nickname() const;
		void		set_nickname(const std::string &nickname);
		std::string	get_username() const;
		void		set_username(const std::string &username);
		std::string	get_realname() const;
		void		set_realname(const std::string &realname);
		void		set_mask();
		std::string get_mask() const;
};
