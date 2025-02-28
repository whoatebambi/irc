/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: florencecousergue <florencecousergue@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:29:01 by fcouserg          #+#    #+#             */
/*   Updated: 2025/02/28 16:28:45 by florencecou      ###   ########.fr       */
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
		
	public:
		Client();
		Client(std::string nickname, std::string username, int fd);
		~Client();
		Client(Client const &src);
		Client &operator=(Client const &src);

		void SetFd(int fd);
		void setIpAdd(std::string ipadd);

		int GetFd();
};
