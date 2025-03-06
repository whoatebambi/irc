/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpointil <jpointil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:29:01 by fcouserg          #+#    #+#             */
/*   Updated: 2025/03/06 14:31:22 by jpointil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>

class Client {
	private:
		int fd; //-> client file descriptor
		std::string nickname;
		std::string username;
		std::string ipadd;
		std::string _saved;
		
	public:
		Client();
		Client(std::string nickname, std::string username, int fd);
		~Client();
		Client(Client const &src);
		Client &operator=(Client const &src);

		void SetFd(int fd);
		void setIpAdd(std::string ipadd);

		// GETTERS
		int getFd() const;
		std::string	getSaved(void) const;


};
