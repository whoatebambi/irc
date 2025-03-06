/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpointil <jpointil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:29:01 by fcouserg          #+#    #+#             */
/*   Updated: 2025/03/06 14:32:16 by jpointil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
{
	this->nickname = "";
	this->username = "";
	this->ipadd = "";
	this->fd = -1;
}
Client::Client(std::string nickname, std::string username, int fd) :fd(fd), nickname(nickname), username(username){}
Client::~Client(){}
Client::Client(Client const &src){*this = src;}
Client &Client::operator=(Client const &src){
	if (this != &src){
		this->nickname = src.nickname;
		this->username = src.username;
		this->ipadd = src.ipadd;
		this->fd = src.fd;
	}
	return (*this);
}

void Client::SetFd(int fd){this->fd = fd;}
void Client::setIpAdd(std::string ipadd){this->ipadd = ipadd;}

int Client::getFd() const {return this->fd;}
std::string Client::getSaved() const {return this->_saved;}