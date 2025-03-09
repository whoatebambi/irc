/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:29:01 by fcouserg          #+#    #+#             */
/*   Updated: 2025/03/09 17:20:40 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"

#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>

class Client {
	private:
		int _fd; //-> client file descriptor
		std::string nickname;
		std::string username;
		std::string ipadd;
		std::string _saved;
		
	public:
		Client();
		Client(std::string nickname, std::string username, int _fd);
		~Client();
		Client(Client const &src);
		Client &operator=(Client const &src);

		void SetFd(int _fd);
		void setIpAdd(std::string ipadd);

		// GETTERS
		int getFd() const;
		std::string	getSaved(void) const;
		

		void	ParseDataClient(int fd);
		void	parse(std::string &line);
		std::string	ft_trim(const std::string &str);
		void	sendMsg(const std::string &msg);

};
