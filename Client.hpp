/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpointil <jpointil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:29:01 by fcouserg          #+#    #+#             */
/*   Updated: 2025/03/10 14:59:21 by jpointil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"
#include "Command.hpp"

#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <map>


class Client {
	private:
		int _fd;
		std::string _nickname;
		std::string _username;
		std::string _ipadd;
		std::string _saved;
        std::map<std::string, Command*> _commandMap;
		
	public:
		Client();
		Client(std::string _nickname, std::string _username, int _fd);
		~Client();
		Client(Client const &src);
		Client &operator=(Client const &src);

		void SetFd(int _fd);
		void set_IpAdd(std::string _ipadd);

		// GETTERS
		int getFd() const;
        std::string getNickname() const;
		std::string	getSaved() const;
		

		void	ParseDataClient(int fd);
		void	parse(std::string &line);
		std::string	ft_trim(const std::string &str);
		//void	CapCommand(const std::string &args);
        void    NickCommand(const std::string &args);
		void    UserCommand(const std::string &args);
};
